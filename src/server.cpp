#include "server.hpp"
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
}

void Server::startListen(int backlog)
{
	if (listen(sockfd, backlog) < 0)
	{
		printf("Falha ao iniciar a escuta (backlog: %d)!\n", backlog, errno);

		return;
	}

	running = true;
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

		char buffer[100];
		auto bytesRead = read(connection, buffer, 100);

		printf("Request: %s", buffer);

		std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nOpa!";

		send(connection, response.c_str(), response.size(), 0);

		close(connection);
	}
}
