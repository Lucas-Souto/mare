#include "Server.hpp"
#include <iostream>
#include <thread>
#include "http/utils.hpp"
#include "lua/export.hpp"
#include "lua/import.hpp"
// SSL
#include <openssl/buffer.h>

Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;
}

void Server::Init(int port, bool https)
{
	this->https = https;

	if (https)
	{
		SSL_library_init();
		OpenSSL_add_all_algorithms();

		const SSL_METHOD* method = SSLv23_server_method();
		ctx = SSL_CTX_new(method);

		if (!ctx)
		{
			printf("Falha ao criar SSL_CTX!\n");

			return;
		}

		SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
		SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);

		if (!SSL_CTX_check_private_key(ctx))
		{
			printf("A chave privada não bate com o certificado público!\n");

			return;
		}
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		printf("Falha ao criar o socket!\n");

		return;
	}

	if (port == 0) port = DEFAULT_PORT;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port);
	int iSetOption = 1;

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		printf("Falha ao anexar o socket na porta %d!\n", port);

		return;
	}

	printf("Socket aberto na porta %d.\nHTTPS: %b\n", port, https);

	running = true;
}

void Server::ResponseTo(int connection, SSL* ssl, char (&buffer)[BUFFER_SIZE])
{
	Request* request = Request::ParseRequest(buffer);
	Route* route = Route::FindMatchingRoute(request->Target, &Routes);
	string response = "";

	printf("%s %s %s\n", request->Method.c_str(), request->URL.c_str(), request->Protocol.c_str());

	if (route != nullptr)
	{
		int targetLength = request->Target->Path.size();

		if (route->Directory != "")
		{
			string file = route->Directory + "/";

			for (int i = route->Path.size(); i < targetLength; i++) file += request->Target->Path[i];

			const char* fileChar = file.c_str();
			string body = getBody(fileChar);

			if (body != "") response = buildResponse(200, getContentType(fileChar), body);
		}
		else response = runCallback(L, route, request);
	}

	if (response == "") response = buildResponse(404, "text/html", Page404);

	if (ssl == nullptr) send(connection, response.c_str(), response.size(), 0);
	else SSL_write(ssl, response.c_str(), response.size());
}

void Server::ListenClients()
{
	auto addrlen = sizeof(sockaddr);
	int connection;
	SSL* ssl;

	while (running)
	{
		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

		if (connection < 0)
		{
			printf("Erro ao aceitar conexão!\n");

			continue;
		}

		if (!https) ssl = nullptr;
		else
		{
			ssl = SSL_new(ctx);

			SSL_set_fd(ssl, connection);

			if (SSL_accept(ssl) <= 0)
			{
				printf("Erro ao aceitar conexão SSL!\n");
				SSL_free(ssl);
				close(connection);

				continue;
			}
		}

		char buffer[BUFFER_SIZE];
		auto bytesRead = https ? SSL_read(ssl, buffer, BUFFER_SIZE) : read(connection, buffer, BUFFER_SIZE);

		if (bytesRead > 0) ResponseTo(connection, ssl, buffer);

		if (https)
		{
			SSL_shutdown(ssl);
			SSL_free(ssl);
		}

		close(connection);
	}
}

void Server::StartListen(int backlog)
{
	if (listen(sockfd, backlog) < 0)
	{
		printf("Falha ao iniciar a escuta (backlog: %d)!\n", backlog);

		return;
	}

	thread lThread(&Server::ListenClients, this);

	lThread.detach();
}

void Server::InitLua()
{
	L = luaL_newstate();

	luaL_openlibs(L);
	lua_register(L, "init", lInit);
	lua_register(L, "listen", lListen);
	lua_register(L, "route", lRoute);
	lua_register(L, "route404", lRoute404);
	lua_register(L, "maskroute", lMaskRoute);
	lua_register(L, "getbody", lGetBody);
	lua_register(L, "render", lRender);
	lua_register(L, "element", lElement);
	lua_register(L, "clear", lClear);
	
	int lua = luaL_loadfile(L, "init.lua");

	if (lua != 0)
	{
		printf("Error: %s.\n", lua_tostring(L, -1));
		lua_pop(L, -1);

		return;
	}

	lua = lua_pcall(L, 0, LUA_MULTRET, 0);

	if (lua != 0)
	{
		printf("Runtime error: %s.\n", lua_tostring(L, -1));
		lua_pop(L, -1);
		
		return;
	}
}

void Server::Close()
{
	if (!running) return;

	running = false;

	close(sockfd);
	lua_close(L);
	SSL_CTX_free(ctx);
}
