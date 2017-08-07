
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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

	return 0;
}

int lua_rotate(lua_State* l) {
	float rads = lua_tonumber(l, 1);
	// rotate(rads);
	LunaEvent event = {};
	event.type = EVENT_ROTATE;
	event.draw.amount = rads;
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

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
	command_queue.push_window_event(event);

	return 0;
}

int lua_present(lua_State *l) {
	LunaEvent event = {};
	event.type = EVENT_PRESENT;
	command_queue.push_window_event(event);

	return 0;
}

// SYS

// int lua_update(lua_State* l) {
	
	
// 	return 0;
// }

// int lua_swap_buffers(lua_State* l) {
// 	rain_swap_buffers(&rain);

// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
// 	return 0;
// }

int lua_sleep(lua_State *l) {
	double t = lua_tonumber(l, 1);

#ifdef __APPLE__
	usleep(t * 1000.0);
#endif
#if _WIN32
	Sleep(t);
#endif
	// LunaEvent event = {};
	// event.type = EVENT_SLEEP;
	// event.sys.time = t;
	// push_event(event);

	return 0;
}

int lua_file_request(lua_State *l) {
	char *f = (char*)lua_tostring(l, 1);
	FileResult file = load_universal_file(f);

	if (file.str) {
		lua_pushstring(l, file.str);
		return 1;
	} else {
		return 0;
	}
}

int lua_get_seconds(lua_State *l) {
	double s = GetSeconds();
	lua_pushnumber(l, s);
	return 1;
}

int lua_key_state(lua_State *l) {
	char *str = (char*)lua_tostring(l, 1);
	if (!str) return 0;
	// printf("%s\n", str);

	bool down = atomic_fetch32((int*)&rain.keys[SDL_GetScancodeFromKey(*str)].down);
	// bool pressed = atomic_fetch32((int*)&rain.keys[SDL_GetScancodeFromKey(*str)].pressed);
	// bool released = atomic_fetch32((int*)&rain.keys[SDL_GetScancodeFromKey(*str)].released);

	// lua_newtable(l);
	// lua_pushstring(l, "down");
	// lua_pushboolean(l, down);
	// lua_settable(l, -3);
	// lua_pushstring(l, "pressed");
	// lua_pushboolean(l, pressed);
	// lua_settable(l, -3);
	// lua_pushstring(l, "released");
	// lua_pushboolean(l, released);
	// lua_settable(l, -3);

	lua_pushboolean(l, down);
	return 1;
}

int lua_next_event(lua_State *l) {
	// LunaEvent event = {};
	// event.input.mouse_button = 1;
	// event.input.mouse_position = {20, 30};
	// event.input.amount = 4;
	// event.input.window_size = {800, 600};

	LunaEvent event = input_queue.pull_event();

	if (event.type != EVENT_NONE) {
		lua_newtable(l);
		lua_pushstring(l, "button"); lua_pushnumber(l, event.input.mouse_button); lua_settable(l, -3);
		lua_pushstring(l, "amount"); lua_pushnumber(l, event.input.amount); lua_settable(l, -3);

		lua_pushstring(l, "pos");
		lua_newtable(l);
		lua_pushstring(l, "x"); lua_pushnumber(l, event.input.mouse_position.x); lua_settable(l, -3);
		lua_pushstring(l, "y"); lua_pushnumber(l, event.input.mouse_position.y); lua_settable(l, -3);
		lua_settable(l, -3);

		lua_pushstring(l, "size");
		lua_newtable(l);
		lua_pushstring(l, "x"); lua_pushnumber(l, event.input.window_size.x); lua_settable(l, -3);
		lua_pushstring(l, "y"); lua_pushnumber(l, event.input.window_size.y); lua_settable(l, -3);
		lua_settable(l, -3);

		lua_pushstring(l, "type");
		switch (event.type) {
			case EVENT_MOUSE_DOWN:
				lua_pushstring(l, "mouse_down");
				break;
			case EVENT_MOUSE_UP:
				lua_pushstring(l, "mouse_up");
				break;
			case EVENT_MOUSE_MOTION:
				lua_pushstring(l, "mouse_motion");
				break;
			case EVENT_MOUSE_WHEEL:
				lua_pushstring(l, "mouse_wheel");
				break;
			case EVENT_WINDOW_RESIZE:
				lua_pushstring(l, "window_resize");
				break;
		}
		lua_settable(l, -3);
	} else {
		lua_pushboolean(l, false);
	}

	return 1;
}

// FONT

int lua_draw_font(lua_State* l) {
	// int fontid = lua_tonumber(l, 1);
	char *font_file = (char*)lua_tostring(l, 1);
	float size = lua_tonumber(l, 2);
	char *str = (char*)lua_tostring(l, 3);
	float x = lua_tonumber(l, 4);
	float y = lua_tonumber(l, 5);
	float width = lua_tonumber(l, 6);

	// _PushFont(font_file, str, {x, y, 0}, size, {1, 1, 1, 1}, width);
	LunaEvent e = {};
	e.type = EVENT_DRAW_FONT;
	e.draw.file = (char*)malloc(strlen(font_file)+1);
	strcpy(e.draw.file, font_file);
	e.draw.scale = size;
	e.draw.str = (char*)malloc(strlen(str)+1);
	strcpy(e.draw.str, str);
	e.draw.pos.x = x;
	e.draw.pos.y = y;
	e.draw.size.x = width;
	command_queue.push_window_event(e);

	return 0;
}

int lua_font_dimensions(lua_State* l) {
	// int fontid = lua_tonumber(l, 1);
	char *font_file = (char*)lua_tostring(l, 1);
	float size = lua_tonumber(l, 2);
	char *str = (char*)lua_tostring(l, 3);
	float width = lua_tonumber(l, 4);

	float2 dim = GetTextDim(font_file, str, size, width);
	// lua_pushnumber(l, dim.x);
	// lua_pushnumber(l, dim.y);
	lua_newtable(l);
	lua_pushstring(l, "x"); lua_pushnumber(l, dim.x); lua_settable(l, -3);
	lua_pushstring(l, "y"); lua_pushnumber(l, dim.y); lua_settable(l, -3);

	return 1;
}

// int lua_load_font(lua_State* l) {
// 	char *file = (char*)lua_tostring(l, 1);
// 	float size = lua_tonumber(l, 2);

// 	int font = LoadFont(file, size);
// 	lua_pushnumber(l, font);

// 	return 1;
// }
