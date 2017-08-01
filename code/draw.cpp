
#include "../../libs/math.c"

bool enable_dynamic_texture_loading = false;
struct {
	GLuint tex;
	char file[256];
} textures[256];
int texture_count = 0;

float2 _tex_coords[4] = {
	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},
};

float current_rotation = 0.0f;

float4 _clear_color = {0, 0, 0, 1};

void draw_line(float x, float y, float x2, float y2) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x2, y2);
	glEnd();
}

void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
	glPopMatrix();
}

void draw_line_triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glEnd();
	glPopMatrix();
}

void draw_rect(float x, float y, float width, float height) {
	float w = width/2.0f;
	float h = height/2.0f;
	glPushMatrix();
	glTranslatef(x + w, y + h, 0.0f);
	glRotatef(todeg(current_rotation), 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-w, -h, 0);
	glVertex3f(w, -h, 0);
	glVertex3f(w, h, 0);
	glVertex3f(-w, h, 0);
	glEnd();
	glPopMatrix();

	current_rotation = 0.0f;
}

void draw_line_rect(float x, float y, float width, float height) {
	float w = width/2.0f;
	float h = height/2.0f;
	glPushMatrix();
	glTranslatef(x + w, y + h, 0.0f);
	glRotatef(todeg(current_rotation), 0, 0, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-w, -h, 0);
	glVertex3f(w, -h, 0);
	glVertex3f(w, h, 0);
	glVertex3f(-w, h, 0);
	glVertex3f(-w, -h, 0);
	glEnd();
	glPopMatrix();

	current_rotation = 0.0f;
}

void set_tex_coords(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	_tex_coords[0] = {x1, y1};
	_tex_coords[1] = {x2, y2};
	_tex_coords[2] = {x3, y3};
	_tex_coords[3] = {x4, y4};
}

void draw_rect_texture(int texture, float x, float y, float width, float height) {
	glBindTexture(GL_TEXTURE_2D, textures[texture].tex);
	glEnable(GL_TEXTURE_2D);

	float w = width/2.0f;
	float h = height/2.0f;
	glPushMatrix();
	glTranslatef(x + w, y + h, 0.0f);
	glRotatef(todeg(current_rotation), 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(_tex_coords[0].x, _tex_coords[0].y);
	glVertex3f(-w, -h, 0.0f);
	glTexCoord2f(_tex_coords[1].x, _tex_coords[1].y);
	glVertex3f(w, -h, 0.0f);
	glTexCoord2f(_tex_coords[2].x, _tex_coords[2].y);
	glVertex3f(w, h, 0.0f);
	glTexCoord2f(_tex_coords[3].x, _tex_coords[3].y);
	glVertex3f(-w, h, 0.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	set_tex_coords(
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f);

	current_rotation = 0.0f;
}

int _load_texture(char *file) {
	int width;
	int height;
	int components;

	FileResult f = load_universal_file(file);
	unsigned char *data = stbi_load_from_memory((stbi_uc*)f.data, f.size, &width, &height, &components, 0);
	if (!data) {
		printf("stb_image: unable to load image, %s", stbi_failure_reason());
		return 0;
	}

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

	// texture_map[texture] = (uint32*)data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

int load_texture(char *file) {
	GLuint texture = 0;
	if (enable_dynamic_texture_loading) {
		texture = _load_texture(file);
	}

	textures[texture_count].tex = texture;
	strcpy(textures[texture_count].file, file);
	return texture_count++;
}

void draw_circle(float x, float y, float w, float h) {
	float rw = (w*0.5f);
	float rh = (h*0.5f);
	int steps = 100;
	glPushMatrix();
	glTranslatef(x + rw, y + rh, 0.0);
	glRotatef(todeg(current_rotation), 0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < steps; i++) {
		float rad = (((float)i / (float)steps) * PI2);
		glVertex3f((cosf(rad)*rw), (sinf(rad)*rh), 0.0f);
	}
	glEnd();
	glPopMatrix();

	current_rotation = 0.0f;
}

void draw_line_circle(float x, float y, float w, float h) {
	float rw = (w*0.5f);
	float rh = (h*0.5f);
	int steps = 100;
	glPushMatrix();
	glTranslatef(x + rw, y + rh, 0.0);
	glRotatef(todeg(current_rotation), 0, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < steps + 1; i++) {
		float rad = (((float)i / (float)steps) * PI2);
		glVertex3f((cosf(rad)*rw), (sinf(rad)*rh), 0.0f);
	}
	glEnd();
	glPopMatrix();

	current_rotation = 0.0f;
}

void rotate(float rads) {
	//glRotatef(rads, 0.0f, 0.0f, 1.0f);
	current_rotation = rads;
}

void clear_rect(float x, float y, float width, float height) {
	float w = width/2.0f;
	float h = height/2.0f;
	glPushAttrib(GL_CURRENT_BIT);
	glColor4f(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
	glPushMatrix();
	glTranslatef(x + w, y + h, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-w, -h, 0);
	glVertex3f(w, -h, 0);
	glVertex3f(w, h, 0);
	glVertex3f(-w, h, 0);
	glEnd();
	glPopMatrix();
	glPopAttrib();
}
