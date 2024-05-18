#include <iostream>

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

int main()
{
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);
	printf("Lua:\n");
	luaL_dofile(L, "test.lua");
	lua_close(L);

	return 0;
}
