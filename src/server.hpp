#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server
{
public:
	const int DEFAULT_PORT = 8000;

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
};
