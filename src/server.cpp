#include "server.hpp"
#include "http.hpp"
#include <iostream>

void Server::init(int port)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		printf("Falha ao criar o socket!\n", errno);
		return;
	}

	if (port == 0) port = DEFAULT_PORT;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		printf("Falha ao anexar o socket na porta %d!\n", port, errno);
		
		return;
	}

	running = true;
}

void Server::startListen(int backlog)
{
	if (!running) return;

	if (listen(sockfd, backlog) < 0)
	{
		printf("Falha ao iniciar a escuta (backlog: %d)!\n", backlog, errno);

		return;
	}

	auto addrlen = sizeof(sockaddr);
	int connection;

	while (running)
	{
		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
		
		if (connection < 0)
		{
			printf("Erro ao aceitar a conexão!\n", errno);

			continue;
		}

		char buffer[BUFFER_SIZE];
		auto bytesRead = read(connection, buffer, BUFFER_SIZE);
		
		if (bytesRead != -1) responseTo(connection, buffer);
		
		close(connection);
	}
}

void Server::responseTo(int connection, char (&buffer)[BUFFER_SIZE])
{
	HTTP* request = HTTP::parse(buffer);

	printf("%s %s %s\n", request->method.c_str(), request->url.c_str(), request->protocol.c_str());

	std::string response = HTTP::buildResponse(200, "text/plain", "Opa!");

	send(connection, response.c_str(), response.size(), 0);
}
