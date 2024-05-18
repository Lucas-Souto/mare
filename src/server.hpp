#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server
{
public:
	static const int DEFAULT_PORT = 8000;
	static const int BUFFER_SIZE = 30720;

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
