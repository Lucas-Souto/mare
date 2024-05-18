#include "src/server.hpp"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

Server* server;

extern "C"
{
	int lInit(lua_State* L)
	{
		int port = lua_tointeger(L, 1);

		server->init(port);

		return 0;
	}

	int lListen(lua_State* L)
	{
		int backlog = lua_tointeger(L, 1);

		server->startListen(backlog);

		return 0;
	}
}

int main()
{
	server = new Server();
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);
	lua_register(L, "init", lInit);
	lua_register(L, "listen", lListen);
	luaL_dofile(L, "init.lua");

	server->stop();
	lua_close(L);

	return 0;
}
