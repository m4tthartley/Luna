#ifndef LUA_H
#define LUA_H

#ifdef _WIN32
	#include <lua.hpp>
#elif __APPLE__
	extern "C" {
		#include <Lua/lua.h>
		#include <Lua/lualib.h>
		#include <Lua/lauxlib.h>
	}
#endif

#include <string>
#include <functional>

class Lua {

public:

	lua_State* l;
	bool error;

	Lua();
	void loadSettings(int &width, int &height, double &viewportScale, bool &fullscreen, std::string &title, bool &limitFrames);

	bool confVar(std::string var);
	bool appVar(std::string var);
	void appSetVar(std::string var, int value);
	void appFunc(std::string func);
	void appFunc(std::string func, int value);

	void registerTable(std::string tableName, const luaL_reg* functions);
	void registerTables();
	
	void registerFunction (std::string table, std::string function, /*std::function<int()>*/lua_CFunction func);

};

#endif