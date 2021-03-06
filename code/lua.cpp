#include "lua.h"

//#ifdef _WIN32
//	#include "../lua/includes/lua.hpp"
//#elif __APPLE__
//	extern "C" {
//		#include <Lua/lua.h>
//		#include <Lua/lualib.h>
//		#include <Lua/lauxlib.h>
//	}
//#endif


//#include "videoLua.h"
//#include "video3.h"
//#include "engine.h"
//#include "audiolua.h"
//#include "shader.h"
//#include "spritebuffer.h"
//#include "shapebuffer.h"
//#include "matrix.h"
//#include "epic_space_game.h"
//#include "imvideo.h"

#define _LUA_BLOCK_ERROR(a, b, c) printf("%s %i %i\n", b, lua_block_top, lua_block_top2);
#define LUA_BLOCK_ERROR(a, b, c) _LUA_BLOCK_ERROR(a, b, c)
#define LUA_BLOCK_BEGIN int lua_block_top = lua_gettop(l);
#define LUA_BLOCK_END int lua_block_top2 = lua_gettop(l); if (lua_block_top!=lua_block_top2) LUA_BLOCK_ERROR(__FILE__, __FUNCTION__, __LINE__)

void Lua::init(char *lua_file) {
	error = false;

	l = lua_open();
	luaL_openlibs(l);

	LUA_BLOCK_BEGIN

	luaL_dostring(l,
		"function vec2(x, y)"
		"	return {"
		"		x = x or 0.0,"
		"		y = y or 0.0,"
		"	}"
		"end\n"
		"function vec3(x, y, z)"
		"	return {"
		"		x = x or 0.0,"
		"		y = y or 0.0,"
		"		z = z or 0.0,"
		"	}"
		"end\n"

		"window = {"
		"	size = vec2(1280, 720),"
		"	title = \"Luna\","
		"	fullscreen = false,"
		"}"

		"mouse = {"
		"	position = vec2(0.0, 0.0),"
		"	position_delta = vec2(0.0, 0.0),"
		"	left = { down = false, pressed = false, released = false },"
		"	right = { down = false, pressed = false, released = false },"
		"	middle = { down = false, pressed = false, released = false },"
		"	wheel_delta = 0,"
		"}"

		"keyboard = {"
		"	n1 = { down = false, pressed = false, released = false },"
		"	n2 = { down = false, pressed = false, released = false },"
		"	n3 = { down = false, pressed = false, released = false },"
		"	n4 = { down = false, pressed = false, released = false },"
		"	n5 = { down = false, pressed = false, released = false },"
		"	n6 = { down = false, pressed = false, released = false },"
		"	n7 = { down = false, pressed = false, released = false },"
		"	n8 = { down = false, pressed = false, released = false },"
		"	n9 = { down = false, pressed = false, released = false },"
		"	n0 = { down = false, pressed = false, released = false },"

		"	a = { down = false, pressed = false, released = false },"
		"	b = { down = false, pressed = false, released = false },"
		"	c = { down = false, pressed = false, released = false },"
		"	d = { down = false, pressed = false, released = false },"
		"	e = { down = false, pressed = false, released = false },"
		"	f = { down = false, pressed = false, released = false },"
		"	g = { down = false, pressed = false, released = false },"
		"	h = { down = false, pressed = false, released = false },"
		"	i = { down = false, pressed = false, released = false },"
		"	j = { down = false, pressed = false, released = false },"
		"	k = { down = false, pressed = false, released = false },"
		"	l = { down = false, pressed = false, released = false },"
		"	m = { down = false, pressed = false, released = false },"
		"	n = { down = false, pressed = false, released = false },"
		"	o = { down = false, pressed = false, released = false },"
		"	p = { down = false, pressed = false, released = false },"
		"	q = { down = false, pressed = false, released = false },"
		"	r = { down = false, pressed = false, released = false },"
		"	s = { down = false, pressed = false, released = false },"
		"	t = { down = false, pressed = false, released = false },"
		"	u = { down = false, pressed = false, released = false },"
		"	v = { down = false, pressed = false, released = false },"
		"	w = { down = false, pressed = false, released = false },"
		"	x = { down = false, pressed = false, released = false },"
		"	y = { down = false, pressed = false, released = false },"
		"	z = { down = false, pressed = false, released = false },"

		"	left = { down = false, pressed = false, released = false },"
		"	right = { down = false, pressed = false, released = false },"
		"	up = { down = false, pressed = false, released = false },"
		"	down = { down = false, pressed = false, released = false },"
		"	lcontrol = { down = false, pressed = false, released = false },"
		"	rcontrol = { down = false, pressed = false, released = false },"
		"	control = { down = false, pressed = false, released = false },"
		"	lshift = { down = false, pressed = false, released = false },"
		"	rshift = { down = false, pressed = false, released = false },"
		"	shift = { down = false, pressed = false, released = false },"
		"	alt = { down = false, pressed = false, released = false },"
		"	caps = { down = false, pressed = false, released = false },"
		"	tab = { down = false, pressed = false, released = false },"
		"	space = { down = false, pressed = false, released = false },"
		"	enter = { down = false, pressed = false, released = false },"
		"	backspace = { down = false, pressed = false, released = false },"
		"	escape = { down = false, pressed = false, released = false },"
		"	f1 = { down = false, pressed = false, released = false },"
		"	f2 = { down = false, pressed = false, released = false },"
		"	f3 = { down = false, pressed = false, released = false },"
		"	f4 = { down = false, pressed = false, released = false },"
		"	f5 = { down = false, pressed = false, released = false },"
		"	f6 = { down = false, pressed = false, released = false },"
		"	f7 = { down = false, pressed = false, released = false },"
		"	f8 = { down = false, pressed = false, released = false },"
		"	f9 = { down = false, pressed = false, released = false },"
		"	f10 = { down = false, pressed = false, released = false },"
		"	f11 = { down = false, pressed = false, released = false },"
		"	f12 = { down = false, pressed = false, released = false },"
		"}"

		"time = {"
		"	dt = 0.0,"
		"	seconds = 0.0,"
		"}"
	);

	registerTables();

	/*if (luaL_dofile(l, "conf.lua") != 0) error = true;
	if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;*/

	//std::string mainpath = "main.lua";
	//if (confVar("mainpath")) mainpath = lua_tostring(l, -1);
	//lua_pop(l, 1);

	// bool error = false;

	// printf("address: %s\n", lua_file);
	FileResult main_file = load_universal_file(lua_file);
	//printf("code: \n%s\n", main_file.str);

	char error_str[256];
	if (main_file.size) {
		if (luaL_dostring(l, main_file.str) != 0) error = true;
		if (lua_type(l, -1) == LUA_TSTRING) {
			error = true;
			strcpy(error_str, (char*)lua_tostring(l, -1));
		}
	} else {
		sprintf(error_str, "Unable to load data from %s\n", lua_file);
		error = true;
	}

	if (error) {
		// std::cout << lua_tostring(l, -1) << std::endl;
		// luaL_dostring(l, "print(debug.traceback)");
		// std::cout << lua_tostring(l, -1) << std::endl;

		// {
		// 	lua_getfield(l, LUA_GLOBALSINDEX, "debug");
		// 	if (!lua_istable(l, -1)) lua_pop(l, 1);
		// 	lua_getfield(l, -1, "traceback");
		// 	if (!lua_isfunction(l, -1)) lua_pop(l, 2);
		// 	lua_pushvalue(l, 1);
		// 	lua_pushinteger(l, 2);
		// 	lua_call(l, 2, 1);
		// }

		char *temp = (char*)malloc(strlen(error_str+1));
		char *error_str2 = (char*)malloc(strlen(error_str+1));
		strcpy(temp, error_str);
		strcpy(error_str2, error_str);
		// error_str = temp;

		char *font = (char*)malloc(strlen(DEBUG_FONT+1));
		strcpy(font, DEBUG_FONT);
		char *font2 = (char*)malloc(strlen(DEBUG_FONT+1));
		strcpy(font2, DEBUG_FONT);
		float2 dim = GetTextDim(font, temp, 1.0f, 0);

		{LunaEvent event = {};
		event.type = EVENT_SET_COLOR;
		event.draw.color = {0, 0, 0, 1};
		command_queue.push_window_event(event); }

		{LunaEvent event = {};
		event.type = EVENT_DRAW_RECT;
		event.draw.pos = {10-5, 10+4-5};
		event.draw.size = {dim.x+10, dim.y+10};
		command_queue.push_window_event(event); }

		{LunaEvent event = {};
		event.type = EVENT_SET_COLOR;
		event.draw.color = {1, 0, 0, 1};
		command_queue.push_window_event(event); }

		{LunaEvent e = {};
		e.type = EVENT_DRAW_FONT;
		e.draw.file = font2;
		e.draw.scale = 1.0f;
		e.draw.str = error_str2;
		e.draw.pos.x = 10;
		e.draw.pos.y = 10;
		e.draw.size.x = rain.window_width - 20;
		command_queue.push_window_event(e);}

		{LunaEvent event = {};
		event.type = EVENT_PRESENT;
		command_queue.push_window_event(event);}

		lua_pop(l, 1);
	}

	LUA_BLOCK_END
}

// bool Lua::confVar(std::string var) {

// 	lua_getfield(l, LUA_GLOBALSINDEX, "conf");
// 	if (lua_istable(l, -1)) {
// 		lua_getfield(l, -1, var.c_str());
// 		if (!lua_isnil(l, -1)) {
// 			return true;
// 		}
// 	}

// 	return false;
// }

char *Lua::get_table_var(char *table, char *var) {
	LUA_BLOCK_BEGIN

	char *result = NULL;
	lua_getfield(l, LUA_GLOBALSINDEX, table);
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, var);
		if (!lua_isnil(l, -1)) {
			result = (char*)lua_tostring(l, -1);
		}
	}

	lua_pop(l, 2);
	LUA_BLOCK_END

	return result;
}

void Lua::set_table_number(char *table, char *var, double num) {
	LUA_BLOCK_BEGIN

	lua_getfield(l, LUA_GLOBALSINDEX, table);
	if (lua_istable(l, -1)) {
		lua_pushnumber(l, num);
		lua_setfield(l, -2, var);
	}

	lua_pop(l, 1);
	LUA_BLOCK_END
}

void Lua::set_table_table_number(char *table, char *table2, char *var, float num) {
	LUA_BLOCK_BEGIN

	lua_getfield(l, LUA_GLOBALSINDEX, table);
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, table2);
		if (lua_istable(l, -1)) {
			lua_pushnumber(l, num);
			lua_setfield(l, -2, var);
		}
	}

	lua_pop(l, 2);
	LUA_BLOCK_END
}

void Lua::set_table_table_bool(char *table, char *table2, char *var, bool b) {
	LUA_BLOCK_BEGIN

	lua_getfield(l, LUA_GLOBALSINDEX, table);
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, table2);
		if (lua_istable(l, -1)) {
			lua_pushboolean(l, b);
			lua_setfield(l, -2, var);
		}
	}

	lua_pop(l, 2);
	LUA_BLOCK_END
}

void Lua::set_table_table_digital_button(char *table, char *table2, digital_button b) {
	LUA_BLOCK_BEGIN

	set_table_table_bool(table, table2, "down", b.down);
	set_table_table_bool(table, table2, "pressed", b.pressed);
	set_table_table_bool(table, table2, "released", b.released);

	LUA_BLOCK_END
}

bool Lua::get_table_table_var(char *table, char *table2, char *var) {
	LUA_BLOCK_BEGIN

	bool result = false;
	lua_getfield(l, LUA_GLOBALSINDEX, table);
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, table2);
		if (lua_istable(l, -1)) {
			lua_getfield(l, -1, var);
			if (!lua_isnil(l, -1)) {
				result = true;
			}
		}
	}

	LUA_BLOCK_END

	return result;
}

bool Lua::appVar(std::string var) {
	LUA_BLOCK_BEGIN

	bool result = false;
	lua_getfield(l, LUA_GLOBALSINDEX, "app");
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, var.c_str());
		if (!lua_isnil(l, -1)) {
			result = true;
		}
	}

	LUA_BLOCK_END

	return result;
}

void Lua::appSetVar(std::string var, int value) {
	LUA_BLOCK_BEGIN

	std::string str = var + " = " + std::to_string(value);

	if (!error)
	if (luaL_dostring(l, str.c_str()) != 0) {
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
	}

	LUA_BLOCK_END
}

void Lua::appFunc(std::string func) {
	LUA_BLOCK_BEGIN

	std::string f = func + "()";

	//if (!error)
	if (luaL_dostring(l, f.c_str()) != 0) {
		std::cout << func << " : ";
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
		//luaL_dostring(l, "print(debug.traceback())");
	}

	LUA_BLOCK_END
}

void Lua::appFunc(std::string func, int value) {
	LUA_BLOCK_BEGIN

	std::string f = func + "(" + std::to_string(value) + ")";

	//if (!error)
	if (luaL_dostring(l, f.c_str()) != 0) {
		std::cout << func << " : ";
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
	}

	LUA_BLOCK_END
}

void Lua::loadSettings(int &width, int &height, double &viewportScale, bool &fullscreen, std::string &title, bool &limitFrames) {
	LUA_BLOCK_BEGIN

	if (appVar("width")) width = lua_tonumber(l, -1);
	if (appVar("height")) height = lua_tonumber(l, -1);
	if (appVar("scale")) viewportScale = lua_tonumber(l, -1);
	if (appVar("fullscreen")) fullscreen = lua_toboolean(l, -1);

	if (appVar("title")) title = lua_tostring(l, -1);
	if (appVar("limitframes")) limitFrames = lua_toboolean(l, -1);

	LUA_BLOCK_END
}

void Lua::registerTable(std::string tableName, const luaL_reg* functions) {
	LUA_BLOCK_BEGIN

	luaL_register(l, tableName.c_str(), functions);

	LUA_BLOCK_END
}

void Lua::create_lua_func(char *name, lua_CFunction func) {
	LUA_BLOCK_BEGIN

	lua_pushcfunction(l, func);
	lua_setglobal(l, name);

	LUA_BLOCK_END
}

void Lua::registerTables() {
	LUA_BLOCK_BEGIN

	create_lua_func("draw_line", lua_draw_line);
	create_lua_func("draw_triangle", lua_draw_triangle);
	create_lua_func("draw_line_triangle", lua_draw_line_triangle);
	create_lua_func("draw_rect", lua_draw_rect);
	create_lua_func("draw_line_rect", lua_draw_line_rect);
	create_lua_func("set_tex_coords", lua_set_tex_coords);
	create_lua_func("draw_rect_texture", lua_draw_rect_texture);
	create_lua_func("draw_circle", lua_draw_circle);
	create_lua_func("draw_line_circle", lua_draw_line_circle);
	create_lua_func("set_color", lua_set_color);
	create_lua_func("load_texture", lua_load_texture);
	create_lua_func("rotate", lua_rotate);
	create_lua_func("clear_rect", lua_clear_rect);
	create_lua_func("clear_color", lua_clear_color);
	create_lua_func("present", lua_present);

	create_lua_func("draw_font", lua_draw_font);
	create_lua_func("font_dimensions", lua_font_dimensions);

	create_lua_func("next_event", lua_next_event);
	create_lua_func("key_state", lua_key_state);
	create_lua_func("mouse_pos", lua_mouse_pos);
	create_lua_func("sleep", lua_sleep);
	create_lua_func("get_seconds", lua_get_seconds);

	create_lua_func("load_module", lua_load_module);
	create_lua_func("http_get", lua_http_get);
	create_lua_func("http_post", lua_http_post);

	// Video
	/*registerFunction("video", "enableTextures", luaEnableTextures);
	registerFunction("video", "clear", luaClear);
	registerFunction("video", "push", luaPush);
	registerFunction("video", "pop", luaPop);
	registerFunction("video", "translate", luaTranslate);
	registerFunction("video", "rotate", luaRotate);
	registerFunction("video", "color", luaColor);
	registerFunction("video", "renderQuad", luaRenderQuad);
	registerFunction("video", "renderSprite", luaRenderSprite);
	registerFunction("video", "renderQuadz", luaRenderQuadz);
	registerFunction("video", "renderSpritez", luaRenderSpritez);
	registerFunction("video", "renderSpriteRotated", luaRenderSpriteRotated);
	registerFunction("video", "renderCircle", luaRenderCircle);
	registerFunction("video", "renderElipse", luaRenderElipse);
	registerFunction("video", "loadTexture", luaLoadTexture);
	registerFunction("video", "loadIdentity", luaLoadIdentity);*/

#if 0
	registerFunction("video", "create_buffer", lua_create_buffer);
	registerFunction("video", "render_buffer", lua_render_buffer);
	
	// Video 2
	registerFunction("SpriteBuffer", "new", SpriteBuffer::lua_spriteBuffer);
	registerFunction("SpriteBuffer", "render", SpriteBuffer::lua_renderSpriteBuffer);
	registerFunction("ShapeBuffer", "new", ShapeBuffer::lua_shapeBuffer);
	registerFunction("ShapeBuffer", "render", ShapeBuffer::lua_renderShapeBuffer);
	
	// Video 3
	registerFunction("video3", "renderQuad", Video3::lua_renderQuad);
	registerFunction("video3", "renderSprite", Video3::lua_renderSprite);
	registerFunction("video3", "renderElipse", Video3::lua_renderElipse);
	registerFunction("video3", "loadIdentity", Video3::lua_loadIdentity);
	registerFunction("video3", "translate", Video3::lua_translate);
	registerFunction("video3", "scale", Video3::lua_scale);
	registerFunction("video3", "rotate", Video3::lua_rotate);
	
	// Shader
	registerFunction("shader", "from_file", Shader::lua_from_file);
	registerFunction("shader", "use", Shader::lua_use);
	//registerFunction("shader", "uniformLocation", Shader::lua_uniformLocation);
	//registerFunction("shader", "uniform3", Shader::lua_uniform3);
	//registerFunction("shader", "uniform4", Shader::lua_uniform4);
	//registerFunction("shader", "uniform1", Shader::lua_uniform1);
	
	// Matrix
	registerFunction("matrix", "identity", Matrix::lua_identity);
	registerFunction("matrix", "translate", Matrix::lua_translate);
	registerFunction("matrix", "rotate", Matrix::lua_rotate);
	registerFunction("matrix", "scale", Matrix::lua_scale);
	
	// Audio
	registerFunction("audio", "loadAudio", AudioLua::loadAudio);
	registerFunction("audio", "play", AudioLua::play);
	
	// Input
	registerFunction("input", "mousex", Engine::getMousex);
	registerFunction("input", "mousey", Engine::getMousey);
	
	// Engine
	registerFunction("engine", "frames", Engine::getFrames);
	registerFunction("engine", "ticks", Engine::getTicks);
	registerFunction("engine", "qps", Engine::getQps);
	
	// Epic Space Game
	registerFunction("esg", "createStarMap", lua_create_star_map);
	registerFunction("esg", "renderStarMap", lua_render_star_map);
	registerFunction("esg", "beginRenderMinimap", lua_begin_render_minimap);
	registerFunction("esg", "endRenderMinimap", lua_end_render_minimap);
	registerFunction("esg", "initMinimap", lua_init_minimap_fbo);
#endif

	LUA_BLOCK_END
}

void Lua::registerFunction (std::string table, std::string function, lua_CFunction func) {
	LUA_BLOCK_BEGIN

	lua_getfield(l, LUA_GLOBALSINDEX, table.c_str());
	if(!lua_istable(l, -1)) {
		
		lua_createtable(l, 0, 1);
		lua_setfield(l, LUA_GLOBALSINDEX, table.c_str());
		lua_pop(l, 1);
		lua_getfield(l, LUA_GLOBALSINDEX, table.c_str());
	}
	
	lua_pushstring(l, function.c_str());
	lua_pushcfunction(l, func);
	lua_settable(l, -3);
	lua_pop(l, 1);

	LUA_BLOCK_END
}