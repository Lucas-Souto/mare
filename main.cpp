#include "src/server.hpp"
#include "src/lua.hpp"

int main()
{
	Server* server = new Server();
	
	createLState(server);
	luaL_dofile(server->L, "init.lua");
	server->stop();

	return 0;
}
