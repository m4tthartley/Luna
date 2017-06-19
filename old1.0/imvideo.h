#ifndef __JellyMoon__imvideo__
#define __JellyMoon__imvideo__

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

#include <map>

struct im_buffer {
	
	GLuint texture;
	float *vertex_buffer;
	float *color_buffer;
	float *texture_buffer;
	int *index_buffer;
	
	int vertex_count;
	int index_count;
	
};

static int buffer_count = 0;
static std::map<int, im_buffer> buffers;

int imvideo_create_buffer (GLuint texture, float *vertices, float *colors, float *texcoords, int *indices, int vertex_count, int index_count);
void imvideo_render_buffer (im_buffer buffer, float x, float y, float z, float angle);

int lua_create_buffer (lua_State *l);
int lua_render_buffer (lua_State *l);

#endif
