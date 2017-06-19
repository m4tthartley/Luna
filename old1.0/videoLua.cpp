#include "videoLua.h"
#include "video.h"

#include <string>
#include <iostream>

int luaEnableTextures(lua_State* l) {

	bool enable = lua_toboolean(l, 1);

	videoEnableTextures(enable);

	return 0;
}

int luaClear(lua_State* l) {
	
	videoClear();

	return 0;
}

int luaPush(lua_State* l) {
	
	videoPush();

	return 0;
}

int luaPop(lua_State* l) {

	videoPop();

	return 0;
}

int luaTranslate(lua_State* l) {

	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double z = lua_tonumber(l, 3);

	videoTranslate(x, y, z);

	return 0;
}

int luaRotate(lua_State* l) {

	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double r = lua_tonumber(l, 3);

	videoRotate(x, y, r);

	return 0;
}

int luaColor(lua_State* l) {

	double r = lua_tonumber(l, 1);
	double g = lua_tonumber(l, 2);
	double b = lua_tonumber(l, 3);
	double a = lua_tonumber(l, 4);

	videoColor(r, g, b, a);

	return 0;
}

int luaRenderQuad(lua_State* l) {

	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double width = lua_tonumber(l, 3);
	double height = lua_tonumber(l, 4);

	videoRenderQuad(x, y, width, height);

	return 0;
}

int luaRenderSprite(lua_State* l) {

	// void videoRenderSprite(GLuint texture, int offsetX, int offsetY, int offsetWidth, int offsetHeight, double x, double y, double scale);

	int texture = lua_tonumber(l, 1);
	int offsetX = lua_tonumber(l, 2);
	int offsetY = lua_tonumber(l, 3);
	int offsetWidth = lua_tonumber(l, 4);
	int offsetHeight = lua_tonumber(l, 5);
	double x = lua_tonumber(l, 6);
	double y = lua_tonumber(l, 7);
	double scale = lua_tonumber(l, 8);

	videoRenderSprite(texture, offsetX, offsetY, offsetWidth, offsetHeight, x, y, scale);

	return 0;
}

int luaRenderQuadz(lua_State* l) {
	
	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double width = lua_tonumber(l, 3);
	double height = lua_tonumber(l, 4);
	double z = lua_tonumber(l, 5);
	
	videoRenderQuadz(x, y, width, height, z);
	
	return 0;
}

int luaRenderSpritez(lua_State* l) {
	
	// void videoRenderSprite(GLuint texture, int offsetX, int offsetY, int offsetWidth, int offsetHeight, double x, double y, double scale);
	
	int texture = lua_tonumber(l, 1);
	int offsetX = lua_tonumber(l, 2);
	int offsetY = lua_tonumber(l, 3);
	int offsetWidth = lua_tonumber(l, 4);
	int offsetHeight = lua_tonumber(l, 5);
	double x = lua_tonumber(l, 6);
	double y = lua_tonumber(l, 7);
	double scale = lua_tonumber(l, 8);
	double z = lua_tonumber(l, 9);
	
	videoRenderSpritez(texture, offsetX, offsetY, offsetWidth, offsetHeight, x, y, scale, z);
	
	return 0;
}

int luaRenderSpriteRotated(lua_State* l) {

	int texture = lua_tonumber(l, 1);
	int offsetX = lua_tonumber(l, 2);
	int offsetY = lua_tonumber(l, 3);
	int offsetWidth = lua_tonumber(l, 4);
	int offsetHeight = lua_tonumber(l, 5);
	double x = lua_tonumber(l, 6);
	double y = lua_tonumber(l, 7);
	double scale = lua_tonumber(l, 8);
	double centerx = lua_tonumber(l, 9);
	double centery = lua_tonumber(l, 10);
	double angle = lua_tonumber(l, 11);

	videoRenderSpriteRotated(texture, offsetX, offsetY, offsetWidth, offsetHeight, x, y, scale, centerx, centery, angle);

	return 0;
}

int luaRenderCircle(lua_State* l) {

	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double radius = lua_tonumber(l, 3);
	double detailScale = lua_tonumber(l, 4);

	videoRenderCircle(x, y, radius, detailScale);

	return 0;
}

int luaRenderElipse(lua_State* l) {

	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double radiusx = lua_tonumber(l, 3);
	double radiusy = lua_tonumber(l, 4);
	double detailScale = lua_tonumber(l, 5);

	videoRenderElipse(x, y, radiusx, radiusy, detailScale);

	return 0;
}

int luaLoadTexture(lua_State* l) {

	std::string file = lua_tostring(l, 1);
	unsigned int number = videoLoadTexture(file);
	lua_pushnumber(l, number);

	return 1;
}

int luaGetTexturePixel (lua_State* l) {
	
	GLuint texture = lua_tonumber(l, 1);
	int x = lua_tonumber(l, 2);
	int y = lua_tonumber(l, 3);
	
	uint32 *data = texture_map[texture];
	std::cout << ((*data) & 0xff) << std::endl;
	//data += y*x + x;
	
	lua_createtable(l, 0, 2);
	lua_pushnumber(l, (*data) & 0xff);
	lua_setfield(l, -2, "r");
	lua_pushnumber(l, (*data >> 8) & 0xff);
	lua_setfield(l, -2, "g");
	lua_pushnumber(l, (*data >> 16) & 0xff);
	lua_setfield(l, -2, "b");
	
	return 1;
}

int luaLoadIdentity(lua_State* l) {

	videoLoadIdentity();

	return 0;
}