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

class SpriteBuffer {
	
	GLuint texture;
	GLuint vbo;
	GLuint ibo;
	
	SpriteBuffer (GLuint texture, GLuint vbo, GLuint ibo);
	SpriteBuffer (GLuint texture, double sx, double sy, double sw, double sh, double size);
	
	void render (GLuint shader, double x, double y, double z, double angle);
	
public:
	
	//Lua
	static int lua_spriteBuffer (lua_State* l);
	static int lua_renderSpriteBuffer (lua_State* l);
	
};