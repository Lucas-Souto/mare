#include "export.hpp"
#include "../Server.hpp"
#include "../http/utils.hpp"

extern "C"
{
	int lInit(lua_State* L)
	{
		if (Server::Get()->IsRunning()) return 0;
		
		Server::Get()->Init(lua_tointeger(L, 1));

		return 0;
	}

	int lListen(lua_State* L)
	{
		if (!Server::Get()->IsRunning()) return 0;

		Server::Get()->StartListen(lua_tointeger(L, 1));

		return 0;
	}

	int lRoute(lua_State* L)
	{
		if (!Server::Get()->IsRunning()) return 0;

		if (lua_gettop(L) < 2) luaL_error(L, "\"route\" espera por dois argumentos (path, callback)!");

		if (lua_isstring(L, 1))
		{
			std::string path(lua_tostring(L, 1));

			if (path[0] != '/') path = "/" + path;

			printf("Criando rota \"%s\"...\n", path.c_str());
			
			if (lua_isfunction(L, 2))
			{
				lua_pushvalue(L, 2);
				(&Server::Get()->Routes)->push_back(new Route(path, luaL_ref(L, LUA_REGISTRYINDEX)));
			}
			else luaL_argerror(L, 2, "\"callback\" precisa ser uma função!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 0;
	}
	int lMaskRoute(lua_State* L)
	{
		if (!Server::Get()->IsRunning()) return 0;

		if (lua_gettop(L) < 2) luaL_error(L, "\"maskroute\" espera por dois argumentos (path, directory)!");

		if (lua_isstring(L, 1))
		{
			std::string path(lua_tostring(L, 1));

			if (path[0] != '/') path = "/" + path;

			printf("Criando rota \"%s\"...\n", path.c_str());

			if (lua_isstring(L, 2)) (&Server::Get()->Routes)->push_back(new Route(path, lua_tostring(L, 2)));
			else luaL_argerror(L, 2, "\"directory\" precisa ser uma string!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 0;
	}
	int lRoute404(lua_State* L)
	{
		if (!Server::Get()->IsRunning()) return 0;

		if (lua_isstring(L, 1))
		{
			std::string page(lua_tostring(L, 1));
			Server::Get()->Page404 = page;
		}
		else luaL_argerror(L, 1, "\"page\" precisa ser uma string!");

		return 0;
	}

	int lGetBody(lua_State* L)
	{
		if (lua_isstring(L, 1)) lua_pushstring(L, getBody(lua_tostring(L, 1)).c_str());
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 1;
	}

	int lRender(lua_State* L) { return 0; }
	int lElement(lua_State* L){ return 0; }
}
