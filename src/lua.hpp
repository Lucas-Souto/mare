#include "server.hpp"
#include "http.hpp"
#include "route.hpp"
#include "response.hpp"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>

	int lInit(lua_State* L);
	int lListen(lua_State* L);
	int lRoute(lua_State* L);
	int lMaskRoute(lua_State* L);
	int lRoute404(lua_State* L);
	int lToBase64(lua_State* L);
	int lRender(lua_State* L);
}

void createLState(Server* server);
Response* runCallback(lua_State* L, FileRoute* route, HTTP* request);
