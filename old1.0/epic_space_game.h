#ifndef __JellyMoon__epic_space_game__
#define __JellyMoon__epic_space_game__

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

void create_star_map (int width, int height, int min_star_size, int max_star_size);
void render_star_map (GLuint shader, double x, double y, double z);

int lua_create_star_map (lua_State *l);
int lua_render_star_map (lua_State *l);

// Minimap
void begin_render_minimap ();
void end_render_minimap ();
void init_minimap_fbo ();
int lua_begin_render_minimap (lua_State *l);
int lua_end_render_minimap (lua_State *l);
int lua_init_minimap_fbo (lua_State *l);

#endif