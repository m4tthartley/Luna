#ifdef _WIN32
	#include <glew.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
#endif

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

class Shader {

public:
	
	static void printLog (GLuint shader, std::string);
	static void compileDefault ();
	
	static GLuint from_file (const char *filename);
	static void use (GLuint shader);
	static GLuint uniformLocation (GLuint shader, std::string uniform);
	
	static void uniform3 (GLuint shader, std::string location, double v0, double v1, double v2);
	static void uniform4 (GLuint shader, std::string location, double v0, double v1, double v2, double v3);
	static void uniform1 (GLuint shader, std::string location, double v0);
	
	// Lua
	static int lua_from_file (lua_State* l);
	static int lua_use (lua_State* l);
	static int lua_uniformLocation (lua_State* l);
	
	static int lua_uniform1 (lua_State* l);
	static int lua_uniform2 (lua_State* l);
	static int lua_uniform3 (lua_State* l);
	static int lua_uniform4 (lua_State* l);
	
	static int lua_uniform (lua_State *l);

};