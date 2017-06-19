#ifndef __JellyMoon__video__
#define __JellyMoon__video__

#include "types.h"

#ifdef _WIN32
	#include <glew.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
#endif

#include <string>
#include <map>

void videoEnableTextures(bool enable);
void videoClear();
void videoPush();
void videoPop();
void videoTranslate(double x, double y, double z);
void videoRotate(double angle);
void videoColor(double r, double g, double b, double a);

void videoRenderQuad(double left, double top, double right, double bottom);
void videoRenderSprite(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale);

void videoRenderQuadz(double left, double top, double right, double bottom, double z);
void videoRenderSpritez(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale, double z);

void videoRenderSpriteRotated(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale, double centerx, double centery, double angle);
void videoRenderTriangleSprite(GLuint texture, double sx, double sy, double sw, double sh, double x, double y, double width, double height);
void videoRenderCircle(double x, double y, double radius, double detailScale);
void videoRenderElipse(double x, double y, double radiusx, double radiusy, double detailScale);
GLuint videoLoadTexture(std::string file);
static std::map<GLuint, uint32*> texture_map;
struct texture_data {
	uint32 *data;
	int width;
	int height;
};
//texture_data videoLoadTextureData(std::string file);

void videoLoadIdentity();
void videoRotate(double x, double y, double angle);

#endif