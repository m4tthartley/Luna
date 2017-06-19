#ifdef _WIN32
	#include <lua.hpp>
#elif __APPLE__
	extern "C" {
		#include <Lua/lua.h>
		#include <Lua/lualib.h>
		#include <Lua/lauxlib.h>
	}
#endif

#include "audio.h"

class AudioLua {
public:
	
	//int init(lua_State* l);
	static int loadAudio(lua_State* l);
	static int play(lua_State* l);
	
};