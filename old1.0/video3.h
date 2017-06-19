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

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class Video3 {
	
private:
	
	static GLuint vertexBuffer;
	static GLuint indexBuffer;
	static GLuint textureBuffer;
	static GLuint elipseVertexBuffer;
	static GLuint elipseIndexBuffer;
	
	static glm::mat4 modelMatrix;
	
public:
	
	static void init ();
	static void renderQuad (GLuint shader, double x, double y, double z, double width, double height);
	static void renderSprite (GLuint shader, GLuint texture, double sx, double sy, double sw, double sh, double x, double y, double z, double scale, double angle);
	static void renderElipse (GLuint shader, double x, double y, double width, double height, double detail);
	
	static void loadIdentity ();
	static void translate (double x, double y, double z);
	static void scale (double x, double y, double z);
	static void rotate (double rotation);
	
	//Lua
	static int lua_renderQuad (lua_State* l);
	static int lua_renderSprite (lua_State* l);
	static int lua_renderElipse (lua_State* l);
	
	static int lua_loadIdentity (lua_State* l);
	static int lua_translate (lua_State* l);
	static int lua_scale (lua_State* l);
	static int lua_rotate (lua_State* l);
	
};