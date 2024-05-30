#include <iostream>
#include "Server.hpp"

int main()
{
	Server* server = new Server();

	server->InitLua();
	
	while (server->IsRunning()) { }

	return 0;
}
