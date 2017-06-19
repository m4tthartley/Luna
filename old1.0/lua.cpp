#include "lua.h"

#ifdef _WIN32
	#include <lua.hpp>
#elif __APPLE__
	extern "C" {
		#include <Lua/lua.h>
		#include <Lua/lualib.h>
		#include <Lua/lauxlib.h>
	}
#endif

#include <iostream>
#include "videoLua.h"
#include "video3.h"
#include "engine.h"
#include "audiolua.h"
#include "shader.h"
#include "spritebuffer.h"
#include "shapebuffer.h"
#include "matrix.h"
#include "epic_space_game.h"
#include "imvideo.h"

Lua::Lua() {

	error = false;

	l = lua_open();
	luaL_openlibs(l);

	registerTables();

	if (luaL_dofile(l, "conf.lua") != 0) error = true;
	if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
	std::string mainpath = "main.lua";
	if (confVar("mainpath")) mainpath = lua_tostring(l, -1);
	lua_pop(l, 1);

	if (luaL_dofile(l, mainpath.c_str()) != 0) error = true;
	if (lua_type(l, -1) == LUA_TSTRING) { std::cout << lua_tostring(l, -1) << std::endl; luaL_dostring(l, "print(debug.traceback)"); std::cout << lua_tostring(l, -1) << std::endl; }
}

bool Lua::confVar(std::string var) {

	lua_getfield(l, LUA_GLOBALSINDEX, "conf");
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, var.c_str());
		if (!lua_isnil(l, -1)) {
			return true;
		}
	}

	return false;
}

bool Lua::appVar(std::string var) {

	lua_getfield(l, LUA_GLOBALSINDEX, "app");
	if (lua_istable(l, -1)) {
		lua_getfield(l, -1, var.c_str());
		if (!lua_isnil(l, -1)) {
			return true;
		}
	}

	return false;
}

void Lua::appSetVar(std::string var, int value) {

	std::string str = var + " = " + std::to_string(value);

	if (!error)
	if (luaL_dostring(l, str.c_str()) != 0) {
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
	}
}

void Lua::appFunc(std::string func) {

	std::string f = func + "()";

	//if (!error)
	if (luaL_dostring(l, f.c_str()) != 0) {
		std::cout << func << " : ";
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
		//luaL_dostring(l, "print(debug.traceback())");
	}
}

void Lua::appFunc(std::string func, int value) {

	std::string f = func + "(" + std::to_string(value) + ")";

	//if (!error)
	if (luaL_dostring(l, f.c_str()) != 0) {
		std::cout << func << " : ";
		if (lua_type(l, -1) == LUA_TSTRING) std::cout << lua_tostring(l, -1) << std::endl;
		error = true;
	}
}

void Lua::loadSettings(int &width, int &height, double &viewportScale, bool &fullscreen, std::string &title, bool &limitFrames) {

	if (appVar("width")) width = lua_tonumber(l, -1);
	if (appVar("height")) height = lua_tonumber(l, -1);
	if (appVar("scale")) viewportScale = lua_tonumber(l, -1);
	if (appVar("fullscreen")) fullscreen = lua_toboolean(l, -1);

	if (appVar("title")) title = lua_tostring(l, -1);
	if (appVar("limitframes")) limitFrames = lua_toboolean(l, -1);
}

void Lua::registerTable(std::string tableName, const luaL_reg* functions) {

	luaL_register(l, tableName.c_str(), functions);
}

void Lua::registerTables() {

	// Video
	registerFunction("video", "enableTextures", luaEnableTextures);
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
	registerFunction("video", "loadIdentity", luaLoadIdentity);
	
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
}

void Lua::registerFunction (std::string table, std::string function, lua_CFunction func) {
	
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
}