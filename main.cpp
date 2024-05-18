#include "src/server.hpp"

int main()
{
	Server* server = new Server();

	server->init(8000);
	server->startListen(10);
	server->stop();

	return 0;
}
