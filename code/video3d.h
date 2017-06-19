#ifndef VIDEO3D_H
#define VIDEO3D_H

#ifdef _WIN32
	#include <glew.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
#endif

class Video3d {

public:

	static void renderSprite(GLuint texture, double sx, double sy, double sw, double sh,
		double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);

};

#endif