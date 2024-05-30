#include "import.hpp"
#include "../http/utils.hpp"
#include <iostream>

string runCallback(lua_State* L, Route* route, Request* request)
{
	if (route->Callback == -1) return "";

	lua_rawgeti(L, LUA_REGISTRYINDEX, route->Callback);
	lua_newtable(L);
	lua_pushstring(L, "route_args");
	lua_newtable(L);

	int index = 0, targetLength = request->Target->Path.size();

	for (int i = route->Path.size(); i < targetLength; i++)
	{
		lua_pushinteger(L, ++index);
		lua_pushstring(L, request->Target->Path[i].substr(1).c_str());
		lua_settable(L, -3);
	}

	lua_settable(L, -3);
	lua_pushstring(L, "method");
	lua_pushstring(L, request->Method.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "host");
	lua_pushstring(L, request->Host.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "url");
	lua_pushstring(L, request->URL.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "languages");
	lua_pushstring(L, request->Languages.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "content_type");
	lua_pushstring(L, request->ContentType.c_str());
	lua_settable(L, -3);
	lua_pushstring(L, "body");
	lua_pushstring(L, request->Body.c_str());
	lua_settable(L, -3);

	if (lua_pcall(L, 1, 1, 0) != LUA_OK)
	{
		printf("------\nFalha ao chamar callback!\n---URL: %s;\n---Error: %s.\n------\n", request->URL.c_str(), lua_tostring(L, -1));
		lua_pop(L, 1);

		return "";
	}

	if (lua_istable(L, -1))
	{
		lua_getfield(L, -1, "status");

		int status = lua_tointeger(L, -1);

		lua_pop(L, 1);
		lua_getfield(L, -1, "content_type");

		const char* contentType = lua_tostring(L, -1);

		lua_pop(L, 1);
		lua_getfield(L, -1, "body");

		const char* body = lua_tostring(L, -1);

		lua_pop(L, 2);

		return buildResponse(status, contentType, body);
	}

	return "";
}
