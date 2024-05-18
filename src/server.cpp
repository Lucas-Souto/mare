#include "server.hpp"
#include "const.hpp"
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

std::string buildResponse(int status, std::string contentType, std::string content)
{
	std::string response = "HTTP/1.1 " + std::to_string(status) + " " + statusText(status) + "\r\nContent-Type: " + contentType;
	response += "\r\nContent-Length: " + std::to_string(content.size()) + "\r\nConnection: Closed\r\n\r\n" + content;

	return response;
}

void Server::responseTo(int connection, char (&buffer)[BUFFER_SIZE])
{
	printf("%s", buffer);

	std::string response = buildResponse(STATUS_OK, TYPE_TEXT, "Opa!");

	send(connection, response.c_str(), response.size(), 0);
}
