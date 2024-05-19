#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include "const.hpp"
#include "route.hpp"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

class Server
{
public:
	lua_State* L;
	std::list<Route*> routes;

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

	Route* findMatchingRoute(std::string url);
	void responseTo(int connection, char (&buffer)[BUFFER_SIZE]);
};
