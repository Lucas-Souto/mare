#include <csignal>
#include "Server.hpp"

Server* server;

void signalHandler(int sig)
{
	if (sig == 2) server->Close();
}

int main()
{
	server = new Server();

	signal(SIGINT, signalHandler);
	server->InitLua();

	printf("Pressione CTRL + C para encerrar o servidor.\n");
	
	while (server->IsRunning()){}

	printf("\nEncerrando servidor...\n");

	return 0;
}
