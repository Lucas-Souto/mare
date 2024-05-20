#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include "const.hpp"
#include "route.hpp"
#include "convert.hpp"

extern "C"
{
	#include <lua.h>
}

class Server
{
public:
	lua_State* L;
	std::list<Route*> routes;

	Server();

	bool isRunning() { return running; }
	static Server* get() { return Server::instance; }
	void init(int port);
	void startListen(int backlog);
	void addRoute(Route* route);
	void stop()
	{
		if (running) close(sockfd);
		
		running = false;
	}
private:
	int sockfd;
	sockaddr_in sockaddr;
	bool running;
	static Server* instance;

	Route* findMatchingRoute(std::string url, std::string (&path)[MAX_DIRECTORIES]);
	void responseTo(int connection, char (&buffer)[BUFFER_SIZE]);
};
#endif
