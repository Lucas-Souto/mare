#include "lua.hpp"
#include "const.hpp"

extern "C"
{
	int lInit(lua_State* L)
	{
		if (Server::get()->isRunning()) return 0;

		int port = lua_tointeger(L, 1);

		Server::get()->init(port);

		return 0;
	}

	int lListen(lua_State* L)
	{
		if (!Server::get()->isRunning()) return 0;

		int backlog = lua_tointeger(L, 1);

		Server::get()->startListen(backlog);

		return 0;
	}

	int lRoute(lua_State* L)
	{
		if (!Server::get()->isRunning()) return 0;

		if (lua_gettop(L) < 2) luaL_error(L, "\"route\" espera por dois argumentos (path, callback)!");

		if (lua_isstring(L, 1))
		{
			std::string path(lua_tostring(L, 1));

			if (path[0] != '/') path = "/" + path;
			
			if (lua_isfunction(L, 2))
			{
				lua_pushvalue(L, 2);
				Server::get()->addRoute(new FileRoute(path, luaL_ref(L, LUA_REGISTRYINDEX)));
			}
			else luaL_argerror(L, 2, "\"callback\" precisa ser uma função!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 0;
	}

	int lRoute404(lua_State* L)
	{
		if (!Server::get()->isRunning()) return 0;

		if (lua_isstring(L, 1))
		{
			std::string page(lua_tostring(L, 1));
			Server::get()->page404 = page;
		}
		else luaL_argerror(L, 1, "\"page\" precisa ser uma string!");

		return 0;
	}

	int lMaskRoute(lua_State* L)
	{
		if (!Server::get()->isRunning()) return 0;

		if (lua_gettop(L) < 2) luaL_error(L, "\"maskroute\" espera por dois argumentos (path, directory)!");

		if (lua_isstring(L, 1))
		{
			std::string path(lua_tostring(L, 1));

			if (path[0] != '/') path = "/" + path;

			if (lua_isstring(L, 2)) Server::get()->addRoute(new AssetRoute(path, lua_tostring(L, 2)));
			else luaL_argerror(L, 2, "\"directory\" precisa ser uma string!");
		}
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 0;
	}

	int lGetBody(lua_State* L)
	{
		if (lua_isstring(L, 1)) lua_pushstring(L, getBody(lua_tostring(L, 1)).c_str());
		else luaL_argerror(L, 1, "\"path\" precisa ser uma string!");

		return 1;
	}

	int lRender(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			const char* path = lua_tostring(L, 1);

			if (lua_istable(L, 2))
			{
				int length = lua_rawlen(L, 2);
				const char* keys[length];
				const char* values[length];

				lua_pushnil(L);

				for (int i = 0; lua_next(L, 2) != 0; i++)	
				{
					keys[i] = lua_tostring(L, -2);
					values[i] = lua_tostring(L, -1);

					lua_pop(L, 1);
				}

				lua_pop(L, 1);
				lua_pushstring(L, render(path, keys, values, length).c_str());
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
			if (lua_isstring(L, 2)) Server::get()->elements.push_back(new HTML(lua_tostring(L, 1), getBody(lua_tostring(L, 2))));
			else luaL_argerror(L, 2, "\"path\" precisa ser uma string!");
		}
		else luaL_argerror(L, 1, "\"tag\" precisa ser uma string!");

		return 0;
	}
}

void createLState(Server* server)
{
	server->L = luaL_newstate();

	luaL_openlibs(server->L);
	lua_register(server->L, "init", lInit);
	lua_register(server->L, "listen", lListen);
	lua_register(server->L, "route", lRoute);
	lua_register(server->L, "route404", lRoute404);
	lua_register(server->L, "maskroute", lMaskRoute);
	lua_register(server->L, "getbody", lGetBody);
	lua_register(server->L, "render", lRender);
	lua_register(server->L, "element", lElement);
}

Response* runCallback(lua_State* L, FileRoute* route, HTTP* request)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, route->callback);
	lua_newtable(L);
	lua_pushstring(L, "route_args");
	lua_newtable(L);

	bool started = false;
	int arrIndex = 0;

	for (int i = 0; i < MAX_DIRECTORIES; i++)
	{
		if (request->path[i] == "") break;

		if (!started)
		{
			if (request->path[i] != route->path[i]) started = true;
		}
		else	
		{
			lua_pushinteger(L, ++arrIndex);
			lua_pushstring(L, request->path[i].c_str());
			lua_settable(L, -3);
		}
	}

	lua_settable(L, -3);
	lua_pushstring(L, "method");
	lua_pushstring(L, request->method.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "languages");
	lua_pushstring(L, request->languages.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "content_type");
	lua_pushstring(L, request->contentType.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "body");
	lua_pushstring(L, request->body.c_str());
	lua_settable(L, -3);

	if (lua_pcall(L, 1, 1, 0) != LUA_OK)
	{
		printf("Falha ao chamar o callback: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);

		return nullptr;
	}

	if (lua_istable(L, -1))
	{
		Response* response = new Response();

		lua_getfield(L, -1, "status");

		response->status = lua_tointeger(L, -1);

		lua_pop(L, 1);
		lua_getfield(L, -1, "content_type");

		response->contentType = lua_tostring(L, -1);

		lua_pop(L, 1);
		lua_getfield(L, -1, "body");

		response->body = lua_tostring(L, -1);

		lua_pop(L, 1);
		lua_pop(L, 1);

		return response;
	}
	
	return nullptr;
}
