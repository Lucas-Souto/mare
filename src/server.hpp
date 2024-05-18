#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "const.hpp"

class Server
{
public:
	void init(int port);
	void startListen(int backlog);
	void stop()
	{
		if (running) close(sockfd);
		
		running = false;
	}
private:
	int sockfd;
	sockaddr_in sockaddr;
	bool running;

	void responseTo(int connection, char (&buffer)[BUFFER_SIZE]);
};
