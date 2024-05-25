#include "src/server.hpp"
#include "src/lua.hpp"

int main()
{
	Server* server = nullptr;

	try
	{
		server = new Server();
		
		createLState(server);
		luaL_dofile(server->L, "init.lua");
	}
	catch (...) { }
	
	if (server != nullptr) server->stop();

	return 0;
}
