#include "video3d.h"

void Video3d::renderSprite(GLuint texture, double sxp, double syp, double swp, double shp,
	double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	int tw;
	int th;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);

	double sx = sxp / tw;
	double sw = swp / tw;
	double sy = syp / th;
	double sh = shp / th;

	glBegin(GL_QUADS);

	glTexCoord2d(sx, sy);
	glVertex3d(x0, y0, z0);

	glTexCoord2d(sx+sw, sy);
	glVertex3d(x1, y1, z1);

	glTexCoord2d(sx+sw, sy+sh);
	glVertex3d(x2, y2, z2);

	glTexCoord2d(sx, sy+sh);
	glVertex3d(x3, y3, z3);

	glEnd();
}