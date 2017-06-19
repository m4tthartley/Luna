#ifdef _WIN32
    #include <lua.hpp>
#elif __APPLE__
	extern "C" {
		#include <Lua/lua.h>
		#include <Lua/lualib.h>
		#include <Lua/lauxlib.h>
	}
#endif

#include "video.h"

int luaEnableTextures(lua_State* l);
int luaClear(lua_State* l);
int luaPush(lua_State* l);
int luaPop(lua_State* l);
int luaTranslate(lua_State* l);
int luaRotate(lua_State* l);
int luaColor(lua_State* l);

int luaRenderQuad(lua_State* l);
int luaRenderSprite(lua_State* l);

int luaRenderQuadz(lua_State* l);
int luaRenderSpritez(lua_State* l);

int luaRenderSpriteRotated(lua_State* l);

int luaRenderCircle(lua_State* l);
int luaRenderElipse(lua_State* l);

int luaLoadTexture(lua_State* l);
int luaGetTexturePixel(lua_State* l);
int luaLoadIdentity(lua_State* l);