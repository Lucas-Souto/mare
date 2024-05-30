#include "Server.hpp"
#include <iostream>
#include <thread>
#include "http/utils.hpp"
#include "lua/export.hpp"
#include "lua/import.hpp"

Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;
}

void Server::Init(int port)
{
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

	printf("Socket aberto na porta %d.\n", port);

	running = true;
}

void Server::ResponseTo(int connection, char (&buffer)[BUFFER_SIZE])
{
	Request* request = Request::ParseRequest(buffer);
	Route* route = Route::FindMatchingRoute(request->Target, &Routes);
	string response = "";

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

	printf("%s %s %s\n", request->Method.c_str(), request->URL.c_str(), request->Protocol.c_str());
	send(connection, response.c_str(), response.size(), 0);

	delete request;
}

void Server::ListenClients()
{
	auto addrlen = sizeof(sockaddr);
	int connection;

	while (running)
	{
		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

		if (connection < 0)
		{
			printf("Erro ao aceitar conexão!\n");

			continue;
		}

		char buffer[BUFFER_SIZE];
		auto bytesRead = read(connection, buffer, BUFFER_SIZE);

		if (bytesRead != -1) ResponseTo(connection, buffer);

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
}
