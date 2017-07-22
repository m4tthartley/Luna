
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
    printf("%s\n", stbi_failure_reason());
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

//////////////////////////////

int lua_draw_line(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float x2 = lua_tonumber(l, 3);
	float y2 = lua_tonumber(l, 4);

	// draw_line(x, y, x2, y2);
	LunaEvent event = {};
	event.type = EVENT_DRAW_LINE;
	event.draw.pos = {x, y};
	event.draw.pos2 = {x2, y2};
	push_event(event);

	return 0;
}

int lua_draw_triangle(lua_State* l) {
	float x1 = lua_tonumber(l, 1);
	float y1 = lua_tonumber(l, 2);
	float x2 = lua_tonumber(l, 3);
	float y2 = lua_tonumber(l, 4);
	float x3 = lua_tonumber(l, 5);
	float y3 = lua_tonumber(l, 6);

	// draw_triangle(x1, y1, x2, y2, x3, y3);
	LunaEvent event = {};
	event.type = EVENT_DRAW_TRIANGLE;
	event.draw.pos = {x1, y1};
	event.draw.pos2 = {x2, y2};
	event.draw.pos3 = {x3, y3};
	push_event(event);

	return 0;
}

int lua_draw_line_triangle(lua_State* l) {
	float x1 = lua_tonumber(l, 1);
	float y1 = lua_tonumber(l, 2);
	float x2 = lua_tonumber(l, 3);
	float y2 = lua_tonumber(l, 4);
	float x3 = lua_tonumber(l, 5);
	float y3 = lua_tonumber(l, 6);

	// draw_line_triangle(x1, y1, x2, y2, x3, y3);
	LunaEvent event = {};
	event.type = EVENT_DRAW_LINE_TRIANGLE;
	event.draw.pos = {x1, y1};
	event.draw.pos2 = {x2, y2};
	event.draw.pos3 = {x3, y3};
	push_event(event);

	return 0;
}

int lua_draw_rect(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	// lua_pushstring(l, "Error in draw_rect");
	// lua_error(l);

	// draw_rect(x, y, width, height);
	LunaEvent event = {};
	event.type = EVENT_DRAW_RECT;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
}

int lua_draw_line_rect(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	// draw_line_rect(x, y, width, height);
	LunaEvent event = {};
	event.type = EVENT_DRAW_LINE_RECT;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
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

	// set_tex_coords(x1, y1, x2, y2, x3, y3, x4, y4);
	LunaEvent event = {};
	event.type = EVENT_SET_TEX_COORDS;
	event.draw.pos = {x1, y1};
	event.draw.pos2 = {x2, y2};
	event.draw.pos3 = {x3, y3};
	event.draw.pos4 = {x4, y4};
	push_event(event);

	return 0;
}

int lua_draw_rect_texture(lua_State* l) {
	int texture = lua_tonumber(l, 1);
	float x = lua_tonumber(l, 2);
	float y = lua_tonumber(l, 3);
	float width = lua_tonumber(l, 4);
	float height = lua_tonumber(l, 5);

	// draw_rect_texture(texture, x, y, width, height);
	LunaEvent event = {};
	event.type = EVENT_DRAW_RECT_TEXTURE;
	event.draw.texture = texture;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
}

int lua_set_color(lua_State* l) {
	float r = lua_tonumber(l, 1);
	float g = lua_tonumber(l, 2);
	float b = lua_tonumber(l, 3);
	float a = lua_tonumber(l, 4);

	// glColor4f(r, g, b, a);
	LunaEvent event = {};
	event.type = EVENT_SET_COLOR;
	event.draw.color = {r, g, b, a};
	push_event(event);

	return 0;
}

int lua_load_texture(lua_State* l) {
	char *file = (char*)lua_tostring(l, 1);
	// unsigned int number = load_texture(file);

	// todo: make sure texture_count isn't being used non-atomicly anywhere
	int index = atomic_add32(&texture_count, 1);
	strcpy(textures[index].file, file);

	LunaEvent event = {};
	event.type = EVENT_LOAD_TEXTURE;
	event.draw.texture = index;
	push_event(event);

	lua_pushnumber(l, index);
	return 1;
}

int lua_draw_circle(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	// draw_circle(x, y, width, height);
	LunaEvent event = {};
	event.type = EVENT_DRAW_CIRCLE;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
}

int lua_draw_line_circle(lua_State* l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	// draw_line_circle(x, y, width, height);
	LunaEvent event = {};
	event.type = EVENT_DRAW_LINE_CIRCLE;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
}

int lua_rotate(lua_State* l) {
	float rads = lua_tonumber(l, 1);
	// rotate(rads);
	LunaEvent event = {};
	event.type = EVENT_ROTATE;
	event.draw.amount = rads;
	push_event(event);

	return 0;
}

int lua_clear_rect(lua_State *l) {
	float x = lua_tonumber(l, 1);
	float y = lua_tonumber(l, 2);
	float width = lua_tonumber(l, 3);
	float height = lua_tonumber(l, 4);

	LunaEvent event = {};
	event.type = EVENT_CLEAR_RECT;
	event.draw.pos = {x, y};
	event.draw.size = {width, height};
	push_event(event);

	return 0;
}

int lua_clear_color(lua_State *l) {
	float r = lua_tonumber(l, 1);
	float g = lua_tonumber(l, 2);
	float b = lua_tonumber(l, 3);
	float a = lua_tonumber(l, 4);

	LunaEvent event = {};
	event.type = EVENT_CLEAR_COLOR;
	event.draw.color = {r, g, b, a};
	push_event(event);

	return 0;
}
