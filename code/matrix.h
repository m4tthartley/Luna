#ifdef _WIN32
	#include <lua.hpp>
#elif __APPLE__
	extern "C" {
		#include <Lua/lua.h>
		#include <Lua/lualib.h>
		#include <Lua/lauxlib.h>
	}
#endif

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

class Matrix {
	
public:
	
	static glm::mat4 modelMatrix;
	
	static void identity ();
	static void translate (double x, double y, double z);
	static void rotate (double angle, double x, double y, double z);
	static void scale (double x, double y, double z);
	
	// Lua
	static int lua_identity (lua_State* l);
	static int lua_translate (lua_State* l);
	static int lua_rotate (lua_State* l);
	static int lua_scale (lua_State* l);
	
};