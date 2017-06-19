#include "audiolua.h"

#include <string>

int AudioLua::loadAudio(lua_State* l) {
	
	std::string file = lua_tostring(l, 1);
	
	unsigned int source = Audio::loadAudio(file);
	lua_pushnumber(l, source);
	
	return 1;
}

int AudioLua::play(lua_State* l) {
	
	int source = lua_tonumber(l, 1);
	
	Audio::play(source);
	
	return 0;
}