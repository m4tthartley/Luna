
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

int lua_draw_rect(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	draw_rect(x, y, width, height);

	return 0;
}

void set_tex_coords(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	_tex_coords[0] = {x1, y1};
	_tex_coords[1] = {x2, y2};
	_tex_coords[2] = {x3, y3};
	_tex_coords[3] = {x4, y4};
}

int lua_set_tex_coords(lua_State* l) {
	float x1 = lua_tonumber(l, 1);
	float y1 = lua_tonumber(l, 2);
	float x2 = lua_tonumber(l, 3);
	float y2 = lua_tonumber(l, 4);
	float x3 = lua_tonumber(l, 5);
	float y3 = lua_tonumber(l, 6);
	float x4 = lua_tonumber(l, 7);
	float y4 = lua_tonumber(l, 8);

	set_tex_coords(x1, y1, x2, y2, x3, y3, x4, y4);

	return 0;
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

int lua_draw_rect_texture(lua_State* l) {
	int texture = lua_tonumber(l, 1);
	float x = lua_tonumber(l, 2);
	float y = lua_tonumber(l, 3);
	float width = lua_tonumber(l, 4);
	float height = lua_tonumber(l, 5);

	draw_rect_texture(texture, x, y, width, height);

	return 0;
}

int lua_set_color(lua_State* l) {
	float r = lua_tonumber(l, 1);
	float g = lua_tonumber(l, 2);
	float b = lua_tonumber(l, 3);
	float a = lua_tonumber(l, 4);

	glColor4f(r, g, b, a);

	return 0;
}

int _load_texture(char *file) {
	int width;
	int height;
	int components;
	unsigned char *data = stbi_load(file, &width, &height, &components, 0);
	if (!data) {
		std::cout << "stb_image: unable to load image" << std::endl;
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

int lua_load_texture(lua_State* l) {
	char *file = (char*)lua_tostring(l, 1);
	unsigned int number = load_texture(file);
	lua_pushnumber(l, number);

	return 1;
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

int lua_draw_circle(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	draw_circle(x, y, width, height);

	return 0;
}

void rotate(float rads) {
	//glRotatef(rads, 0.0f, 0.0f, 1.0f);
	current_rotation = rads;
}

int lua_rotate(lua_State* l) {
	float rads = lua_tonumber(l, 1);
	rotate(rads);
	return 0;
}