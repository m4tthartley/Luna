#include "video.h"
#include "types.h"

//#ifdef _WIN32
//	#include <SDL_image.h>
//#elif __APPLE__
//	#include <SDL2_image/SDL_image.h>
//#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)

extern "C" {
	#include "../stb/stb_image.h"
}
#include <iostream>

void videoEnableTextures(bool enable) {

	if (enable) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);
}

void videoClear() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void videoPush() {

	glPushMatrix();
}

void videoPop() {

	glPopMatrix();
}

void videoTranslate(double x, double y, double z) {

	glTranslated(x, y, z);
}

void videoRotate(double angle) {

	glRotated(angle, 0.0, 0.0, 1.0);
}

void videoColor(double r, double g, double b, double a) {

	glColor4f(r, g, b, a);
}

void videoRenderQuad(double x, double y, double width, double height) {

	glBegin(GL_QUADS);

	glVertex3d(x, y, 0);
	glVertex3d(x + width, y, 0);
	glVertex3d(x + width, y + height, 0);
	glVertex3d(x, y + height, 0);

	glEnd();
}

void videoRenderSprite(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale) {

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	int tw;
	int th;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);

	double xStart = offsetX / tw;
	double xEnd = (offsetX+offsetWidth) / tw;
	double yStart = offsetY / th;
	double yEnd = (offsetY + offsetHeight) / th;
	double width = offsetWidth*scale;
	double height = offsetHeight*scale;

	glBegin(GL_QUADS);

	glTexCoord2d(xStart, yStart);
	glVertex3f(x, y, 0);

	glTexCoord2d(xEnd, yStart);
	glVertex3f(x + width, y, 0);

	glTexCoord2d(xEnd, yEnd);
	glVertex3f(x + width, y + height, 0);

	glTexCoord2d(xStart, yEnd);
	glVertex3f(x, y + height, 0);

	glEnd();
}

void videoRenderQuadz(double x, double y, double width, double height, double z) {
	
	glBegin(GL_QUADS);
	
	glVertex3d(x, y, z);
	glVertex3d(x + width, y, z);
	glVertex3d(x + width, y + height, z);
	glVertex3d(x, y + height, z);
	
	glEnd();
}

void videoRenderSpritez(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale, double z) {
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	
	int tw;
	int th;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
	
	double xStart = offsetX / tw;
	double xEnd = (offsetX+offsetWidth) / tw;
	double yStart = offsetY / th;
	double yEnd = (offsetY + offsetHeight) / th;
	double width = offsetWidth*scale;
	double height = offsetHeight*scale;
	
	glBegin(GL_QUADS);
	
	glTexCoord2d(xStart, yStart);
	glVertex3f(x, y, z);
	
	glTexCoord2d(xEnd, yStart);
	glVertex3f(x + width, y, z);
	
	glTexCoord2d(xEnd, yEnd);
	glVertex3f(x + width, y + height, z);
	
	glTexCoord2d(xStart, yEnd);
	glVertex3f(x, y + height, z);
	
	glEnd();
}

void videoRenderSpriteRotated(GLuint texture, double offsetX, double offsetY, double offsetWidth, double offsetHeight, double x, double y, double scale, double centerx, double centery, double angle) {

	videoPush();

	videoTranslate(x, y, 0);
	videoRotate(centerx, centery, angle);
	videoRenderSprite(texture, offsetX, offsetY, offsetWidth, offsetHeight, 0, 0, scale);

	videoPop();
}

//void videoRenderTriangleSprite(GLuint texture, double sx, double sy, double sw, double sh, double x, double y, double width, double height) {
//
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glEnable(GL_TEXTURE_2D);
//
//	int tw;
//	int th;
//	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
//	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
//
//	double xStart = offsetX / tw;
//	double xEnd = (offsetX + offsetWidth) / tw;
//	double yStart = offsetY / th;
//	double yEnd = (offsetY + offsetHeight) / th;
//	double width = offsetWidth*scale;
//	double height = offsetHeight*scale;
//}

void videoRenderCircle(double x, double y, double radius, double detailScale) {

	int detail = 50*detailScale;

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < detail; i++) {

		double rad = (((double)i/(double)detail)*360)*(3.14159 / 180.0);
		//glVertex3d( x+(cos(rad)*radius), y+(sin(rad)*radius), 0 );
	}

	glEnd();
}

void videoRenderElipse(double x, double y, double radiusx, double radiusy, double detailScale) {
	
	int detail = 50 * detailScale;

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < detail; i++) {

		double rad = (((double)i / (double)detail) * 360)*(3.14159 / 180.0);
		//glVertex3d(x + (cos(rad)*radiusx), y + (sin(rad)*radiusy), 0);
	}

	glEnd();
}

GLuint videoLoadTexture(std::string file) {

//	SDL_Surface *surface = IMG_Load(file.c_str());
//	if (surface == nullptr) {
//		std::cout << "Bitmap load error: " << SDL_GetError() << std::endl;
//		return NULL;
//	}
	
	int width;
	int height;
	int components;
	unsigned char *data = stbi_load(file.c_str(), &width, &height, &components, 0);
	if (data == nullptr) {
		std::cout << "stb_image: unable to load image" << std::endl;
		return 0;
	}
	//std::cout << file << " " << width << " " << height << " " << components << std::endl;
	//unsigned int *i = (unsigned int*)data;
	//std::cout << "texture int " << ((*i >> 16) & 0xff) << " " << ((*i >> 8) & 0xff) << " " << ((*i) & 0xff) << std::endl;
	//i++;
	//std::cout << "texture int " << ((*i >> 16) & 0xff) << " " << ((*i >> 8) & 0xff) << " " << ((*i) & 0xff) << std::endl;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int iFormat = GL_RGB;
	if (components == 4) iFormat = GL_RGBA;
#ifdef __APPLE__
	int format = GL_RGB;
	if (components == 4) format = GL_RGBA;
#elif _WIN32
	int format = iFormat;
#endif

	glTexImage2D(
		GL_TEXTURE_2D, 0, iFormat,
		width, height,
		0,
		format, GL_UNSIGNED_BYTE, data
	);

	texture_map[texture] = (uint32*)data;
	//SDL_FreeSurface(surface);
	//stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//std::cout << "texture: " << texture << std::endl;
	return texture;
}

//texture_data videoLoadTextureData (std::string file) {
//	
//	texture_data data = {};
//	
//	int components;
//	data.data = (uint32*)stbi_load(file.c_str(), &data.width, &data.height, &components, 0);
//	if (data.data == nullptr) {
//		std::cout << "stb_image: unable to load image" << std::endl;
//		return data;
//	}
//	
//	return data;
//}

void videoLoadIdentity() {

	glLoadIdentity();
}

void videoRotate(double x, double y, double angle) {

	videoTranslate(x, y, 0);
	videoRotate(angle);
	videoTranslate(-x, -y, 0);
}