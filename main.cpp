#include "src/server.hpp"

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

	int lRoute(lua_State* L)
	{
		if (lua_gettop(L) < 2) luaL_error(L, "\"route\" espera por dois argumentos (path, callback)!");

		if (lua_isstring(L, 1))
		{
			std::string path(lua_tostring(L, 1));

			if (path[0] == '/')
			{
				if (lua_isfunction(L, 2))
				{
					lua_pushvalue(L, 2);
					server->addRoute(new FileRoute(path, luaL_ref(L, LUA_REGISTRYINDEX)));
				}
				else luaL_argerror(L, 2, "\"callback\" precisa ser uma função!");
			}
			else luaL_argerror(L, 1, "\"path\" precisa começar com \"/\"!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 0;
	}

	int lRoute404(lua_State* L)
	{
		

		return 0;
	}

	int lMaskRoute(lua_State* L)
	{
		

		return 0;
	}
}

int main()
{
	server = new Server();
	server->L = luaL_newstate();

	luaL_openlibs(server->L);
	lua_register(server->L, "init", lInit);
	lua_register(server->L, "listen", lListen);
	lua_register(server->L, "route", lRoute);
	lua_register(server->L, "route404", lRoute404);
	lua_register(server->L, "maskroute", lMaskRoute);
	luaL_dofile(server->L, "init.lua");

	server->stop();
	lua_close(server->L);

	return 0;
}
