#ifndef EXPORT
#define EXPORT
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
	int lGetBody(lua_State* L);
	int lRender(lua_State* L);
	int lElement(lua_State* L);
}
#endif
