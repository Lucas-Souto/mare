#include "server.hpp"
#include "http.hpp"
#include "lua.hpp"

Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;
}

void Server::init(int port)
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

	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		printf("Falha ao anexar o socket na porta %d!\n", port);
		
		return;
	}
	else printf("Socket aberto na porta %d!\n", port);

	running = true;
}

void Server::startListen(int backlog)
{
	if (!running) return;

	if (listen(sockfd, backlog) < 0)
	{
		printf("Falha ao iniciar a escuta (backlog: %d)!\n", backlog);

		return;
	}

	auto addrlen = sizeof(sockaddr);
	int connection;

	while (running)
	{
		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
		
		if (connection < 0)
		{
			printf("Erro ao aceitar a conexão!\n");

			continue;
		}

		char buffer[BUFFER_SIZE];
		auto bytesRead = read(connection, buffer, BUFFER_SIZE);
		
		if (bytesRead != -1) responseTo(connection, buffer);
		
		close(connection);
	}
}

void Server::addRoute(Route* route)
{
	routes.push_back(route);
}

Route* Server::findMatchingRoute(std::string url, std::string (&path)[MAX_DIRECTORIES])
{
	int match = 0, lastMatch = 0;// Quantidade de subrotas que deram match
	Route* result = nullptr;

	Route::buildPath(url, path);

	for (Route* route : this->routes)
	{
		if (path[0] == "/")
		{
			if (route->path[0] == "/")
			{
				result = route;

				break;
			}
		}
		else for (int i = 0; i < MAX_DIRECTORIES; i++)
		{
			if (path[i] == "" || route->path[i] == "")
			{
				match = 0;

				continue;
			}
			else
			{
				if (path[i] == route->path[i])				
				{
					match++;

					if (match > lastMatch)
					{
						lastMatch = match;
						result = route;
					}
				}
				else
				{
					match = 0;
					
					continue;
				}
			}
		}
	}

	return result;
}

void Server::responseTo(int connection, char (&buffer)[BUFFER_SIZE])
{
	HTTP* request = HTTP::parse(buffer);

	printf("%s %s %s\n", request->method.c_str(), request->url.c_str(), request->protocol.c_str());
	
	Route* route = findMatchingRoute(request->url, request->path);
	std::string response;
	bool error404 = route == nullptr;

	if (!error404)
	{
		if (route->getType() == 'f')
		{
			Response* re = runCallback(L, (FileRoute*)route, request);
			
			if (re != nullptr) response = HTTP::buildResponse(re->status, re->contentType, re->body);
			else if (re->contentType == "text/html") error404 = true;
			else response = HTTP::buildResponse(404, "text/plain", "404");
		}
		else
		{
			std::string file = ((AssetRoute*)route)->directory + "/";
			
			for (int i = 0; i < MAX_DIRECTORIES; i++)
			{
				if (request->path[i] == "") break;
				else if (request->path[i] != route->path[i]) file += request->path[i];
			}
			
			const char* fileC = file.c_str();
			std::string body = getBody(fileC);

			if (body == "") response = HTTP::buildResponse(404, "text/plain", "404");
			else response = HTTP::buildResponse(200, getContentType(fileC), body);
		}
	}
	
	if (error404) response = HTTP::buildResponse(404, "text/html", page404);

	send(connection, response.c_str(), response.size(), 0);
}
