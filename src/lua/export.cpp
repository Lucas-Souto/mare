#include "export.hpp"
#include "../Server.hpp"
#include "../http/utils.hpp"
#include "../LinkedPair.hpp"
#include "../HTML.hpp"

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

	LinkedPair* fillTable(lua_State* L, int index, std::string prefix, LinkedPair* dict)
	{
		lua_pushnil(L);

		while (lua_next(L, index) != 0)	
		{
			std::string key(lua_tostring(L, -2));
			
			if (lua_istable(L, -1)) dict = fillTable(L, lua_gettop(L), prefix + key + ".", dict);
			else
			{
				std::string value;

				if (lua_isboolean(L, -1)) value = lua_toboolean(L, -1) ? "true" : "false";
				else value = lua_tostring(L, -1);

				dict->Set(VAR_INDICATOR + prefix + key + VAR_INDICATOR, value);

				dict->Next = new LinkedPair();
				dict = dict->Next;
			}

			lua_pop(L, 1);
		}

		return dict;
	}

	int lRender(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			const char* path = lua_tostring(L, 1);

			if (lua_istable(L, 2))
			{
				LinkedPair* dict = new LinkedPair();

				fillTable(L, 2, "", dict);
				lua_pushstring(L, render(path, dict).c_str());
			}
			else luaL_argerror(L, 2, "\"data\" precisa ser uma tabela!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 1;
	}
	int lElement(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			if (lua_isstring(L, 2))
			{
				HTML* element = new HTML(lua_tostring(L, 1), getBody(lua_tostring(L, 2)));

				if (lua_gettop(L) > 2)
				{
					if (lua_isstring(L, 3)) element->Style = lua_tostring(L, 3);
					else luaL_argerror(L, 3, "\"style\" precisa ser uma string!");

					if (lua_isstring(L, 4)) element->Script = lua_tostring(L, 4);
					else luaL_argerror(L, 4, "\"script\" precisa ser uma string!");
				}

				Server::Get()->Elements.push_back(element);
			}
			else luaL_argerror(L, 2, "\"path\" precisa ser uma string!");
		}
		else luaL_argerror(L, 1, "\"tag\" precisa ser uma string!");

		return 0;
	}
}
