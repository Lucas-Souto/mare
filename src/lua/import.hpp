#ifndef IMPORT
#define IMPORT
#include <string>
#include "../http/Route.hpp"
#include "../http/Request.hpp"

using namespace std;

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

string runCallback(lua_State* L, Route* route, Request* request);
#endif
