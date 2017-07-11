
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

static int mousex;
static int mousey;
static int fps;
static int tps;
static int qps;
static void addqps(int num) { qps += num; }

struct Engine {

	/*SDL_Window *window;
	SDL_GLContext glContext;
	SDL_DisplayMode desktopDisplayMode;*/
	Lua lua;

	int width = 1280;
	int height = 720;
	double viewportScale = 1;
	bool fullscreen = false;
	std::string title;
	bool limitFrames = true;

	// char *default_lua_file = "main.lua";

	bool reload = false;

	/*Engine();
	void run();
	int time();
	void printVersionInfo();

	static int getMousex(lua_State* l);
	static int getMousey(lua_State* l);
	static int getFrames(lua_State* l);
	static int getTicks(lua_State* l);
	static int getQps(lua_State* l);*/

	Rain rain = {};

	void run() {
		lua.init(_address);
		lua.appFunc("init");
		/*if (lua.get_table_table_var("window", "size", "x")) rain.window_width = lua_tonumber(lua.l, -1);
		if (lua.get_table_table_var("window", "size", "y")) rain.window_height = lua_tonumber(lua.l, -1);*/
		rain.window_title = lua.get_table_var("window", "title");

		rain_init(&rain);

		enable_dynamic_texture_loading = true;
		for (int i = 0; i < texture_count; ++i) {
			textures[i].tex = _load_texture(textures[i].file);
		}

		//lua.loadSettings(width, height, viewportScale, fullscreen, title, limitFrames);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);
		glEnable(GL_ALPHA_TEST);

		//glViewport(0, 0, (int)(width*viewportScale), (int)(height*viewportScale));
		glViewport(0, 0, rain.window_width, rain.window_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		/*glOrtho(0, width, height, 0, -100, 100);*/
		glOrtho(0, rain.window_width, rain.window_height, 0, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		init_font_system();

		do {
			if (reload) {
				for (int i = 0; i < texture_count; ++i) {
					glDeleteTextures(1, &textures[i].tex);
				}
				texture_count = 0;
				lua_close(lua.l);
				lua = {};
				lua.init(_address);
				lua.appFunc("init");
				reload = false;
			}
			lua.appFunc("run");
		} while (reload);

#if 0
		//		int test_font = LoadFont("Jellee-Roman.ttf", 1.5f);
		while (!rain.quit) {
			//			float2 size = GetTextDim(test_font, "Hello, I'm testing the sweet fonts. Do you like my sweet ass fonts?", {1, 1}, 300);
			//			draw_rect(10, 10, size.x, size.y);
			//			_PushFont(test_font, "Hello, I'm testing the sweet fonts. Do you like my sweet ass fonts?", {10, 10}, {4, 4}, {0, 1, 1, 1}, 300);



			double time_frame = GetSeconds();
			// rain_update(&rain);

			

			

			/*glBegin(GL_QUADS);
			glColor4f(1.0, 0.0, 1.0, 1.0);
			glVertex2f(-1.0, -1.0);
			glVertex2f(1.0, -1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(-1.0, 1.0);
			glEnd();*/

			

			double time_lua_update = GetSeconds();
			lua.appFunc("update");
			time_lua_update = GetSeconds() - time_lua_update;

			time_frame = GetSeconds() - time_frame;
			//printf("time, frame %f, lua state %f, lua update %f \n", time_frame*1000.0, time_lua_state*1000.0, time_lua_update*1000.0);
		}
#endif
	}

	/*void printVersionInfo() {

		SDL_version sdlVersion;
		SDL_GetVersion(&sdlVersion);
		std::cout << "sdl version(" << std::to_string(sdlVersion.major) <<
			" " << std::to_string(sdlVersion.minor) << ")" << std::endl;
		std::cout << "gl(" << glGetString(GL_VERSION) << ") glsl(" << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << std::endl;
	}*/
};

Engine _engine;

int lua_update(lua_State* l) {
	rain_poll_input(&_engine.rain);
	rain_poll_time(&_engine.rain);

	if (_engine.rain.quit) {
		// Force the lua run loop to quit
		lua_pushstring(_engine.lua.l, "Quitting program...");
		lua_error(_engine.lua.l);
	}

	static bool reload_shortcut = false;
#ifdef _WIN32
	if (GetAsyncKeyState(VK_CONTROL) && (GetAsyncKeyState('S') || GetAsyncKeyState('R'))) {
		if (!reload_shortcut) {
			for (int i = 0; i < texture_count; ++i) {
				glDeleteTextures(1, &textures[i].tex);
			}
			texture_count = 0;
			lua_close(lua.l);
			lua = Lua(default_lua_file);
			lua.appFunc("init");
		}
		reload_shortcut = true;
	} else {
		reload_shortcut = false;
	}
#endif
#ifdef __APPLE__
	if ((_engine.rain.keys[KEY_CTRL].down || _engine.rain.keys[KEY_LGUI].down) && (_engine.rain.keys[KEY_S].pressed || _engine.rain.keys[KEY_R].pressed)) {
		if (!reload_shortcut) {
			// for (int i = 0; i < texture_count; ++i) {
			// 	glDeleteTextures(1, &textures[i].tex);
			// }
			// texture_count = 0;
			// lua_close(lua.l);
			// lua = {};
			// lua.init(address);
			// lua.appFunc("init");
			_engine.reload = true;
			lua_pushstring(_engine.lua.l, "reloading...");
			lua_error(_engine.lua.l);
		}
		reload_shortcut = true;
	} else {
		reload_shortcut = false;
	}
#endif

	glViewport(0, 0, _engine.rain.window_width, _engine.rain.window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _engine.rain.window_width, _engine.rain.window_height, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	return 0;
}

int lua_get_input(lua_State* l) {
	_engine.lua.set_table_table_number("window", "size", "x", _engine.rain.window_width);
	_engine.lua.set_table_table_number("window", "size", "y", _engine.rain.window_height);

	double time_lua_state = GetSeconds();
	_engine.lua.set_table_table_number("mouse", "position", "x", _engine.rain.mouse.position.x);
	_engine.lua.set_table_table_number("mouse", "position", "y", _engine.rain.mouse.position.y);
	_engine.lua.set_table_table_number("mouse", "position_delta", "x", _engine.rain.mouse.position_delta.x);
	_engine.lua.set_table_table_number("mouse", "position_delta", "y", _engine.rain.mouse.position_delta.y);

	_engine.lua.set_table_table_bool("mouse", "left", "down", _engine.rain.mouse.left.down);
	_engine.lua.set_table_table_bool("mouse", "left", "pressed", _engine.rain.mouse.left.pressed);
	_engine.lua.set_table_table_bool("mouse", "left", "released", _engine.rain.mouse.left.released);

	_engine.lua.set_table_table_bool("mouse", "right", "down", _engine.rain.mouse.right.down);
	_engine.lua.set_table_table_bool("mouse", "right", "pressed", _engine.rain.mouse.right.pressed);
	_engine.lua.set_table_table_bool("mouse", "right", "released", _engine.rain.mouse.right.released);

	_engine.lua.set_table_table_bool("mouse", "middle", "down", _engine.rain.mouse.middle.down);
	_engine.lua.set_table_table_bool("mouse", "middle", "pressed", _engine.rain.mouse.middle.pressed);
	_engine.lua.set_table_table_bool("mouse", "middle", "released", _engine.rain.mouse.middle.released);

	_engine.lua.set_table_number("mouse", "wheel_delta", _engine.rain.mouse.wheel_delta);

	{
		_engine.lua.set_table_table_digital_button("keyboard", "n1", _engine.rain.keys[KEY_1]);
		_engine.lua.set_table_table_digital_button("keyboard", "n2", _engine.rain.keys[KEY_2]);
		_engine.lua.set_table_table_digital_button("keyboard", "n3", _engine.rain.keys[KEY_3]);
		_engine.lua.set_table_table_digital_button("keyboard", "n4", _engine.rain.keys[KEY_4]);
		_engine.lua.set_table_table_digital_button("keyboard", "n5", _engine.rain.keys[KEY_5]);
		_engine.lua.set_table_table_digital_button("keyboard", "n6", _engine.rain.keys[KEY_6]);
		_engine.lua.set_table_table_digital_button("keyboard", "n7", _engine.rain.keys[KEY_7]);
		_engine.lua.set_table_table_digital_button("keyboard", "n8", _engine.rain.keys[KEY_8]);
		_engine.lua.set_table_table_digital_button("keyboard", "n9", _engine.rain.keys[KEY_9]);
		_engine.lua.set_table_table_digital_button("keyboard", "n0", _engine.rain.keys[KEY_0]);
		_engine.lua.set_table_table_digital_button("keyboard", "a", _engine.rain.keys[KEY_A]);
		_engine.lua.set_table_table_digital_button("keyboard", "b", _engine.rain.keys[KEY_B]);
		_engine.lua.set_table_table_digital_button("keyboard", "c", _engine.rain.keys[KEY_C]);
		_engine.lua.set_table_table_digital_button("keyboard", "d", _engine.rain.keys[KEY_D]);
		_engine.lua.set_table_table_digital_button("keyboard", "e", _engine.rain.keys[KEY_E]);
		_engine.lua.set_table_table_digital_button("keyboard", "f", _engine.rain.keys[KEY_F]);
		_engine.lua.set_table_table_digital_button("keyboard", "g", _engine.rain.keys[KEY_G]);
		_engine.lua.set_table_table_digital_button("keyboard", "h", _engine.rain.keys[KEY_H]);
		_engine.lua.set_table_table_digital_button("keyboard", "i", _engine.rain.keys[KEY_I]);
		_engine.lua.set_table_table_digital_button("keyboard", "j", _engine.rain.keys[KEY_J]);
		_engine.lua.set_table_table_digital_button("keyboard", "k", _engine.rain.keys[KEY_K]);
		_engine.lua.set_table_table_digital_button("keyboard", "l", _engine.rain.keys[KEY_L]);
		_engine.lua.set_table_table_digital_button("keyboard", "m", _engine.rain.keys[KEY_M]);
		_engine.lua.set_table_table_digital_button("keyboard", "n", _engine.rain.keys[KEY_N]);
		_engine.lua.set_table_table_digital_button("keyboard", "o", _engine.rain.keys[KEY_O]);
		_engine.lua.set_table_table_digital_button("keyboard", "p", _engine.rain.keys[KEY_P]);
		_engine.lua.set_table_table_digital_button("keyboard", "q", _engine.rain.keys[KEY_Q]);
		_engine.lua.set_table_table_digital_button("keyboard", "r", _engine.rain.keys[KEY_R]);
		_engine.lua.set_table_table_digital_button("keyboard", "s", _engine.rain.keys[KEY_S]);
		_engine.lua.set_table_table_digital_button("keyboard", "t", _engine.rain.keys[KEY_T]);
		_engine.lua.set_table_table_digital_button("keyboard", "u", _engine.rain.keys[KEY_U]);
		_engine.lua.set_table_table_digital_button("keyboard", "v", _engine.rain.keys[KEY_V]);
		_engine.lua.set_table_table_digital_button("keyboard", "w", _engine.rain.keys[KEY_W]);
		_engine.lua.set_table_table_digital_button("keyboard", "x", _engine.rain.keys[KEY_X]);
		_engine.lua.set_table_table_digital_button("keyboard", "y", _engine.rain.keys[KEY_Y]);
		_engine.lua.set_table_table_digital_button("keyboard", "z", _engine.rain.keys[KEY_Z]);

		_engine.lua.set_table_table_digital_button("keyboard", "left", _engine.rain.keys[KEY_LEFT]);
		_engine.lua.set_table_table_digital_button("keyboard", "right", _engine.rain.keys[KEY_RIGHT]);
		_engine.lua.set_table_table_digital_button("keyboard", "up", _engine.rain.keys[KEY_UP]);
		_engine.lua.set_table_table_digital_button("keyboard", "down", _engine.rain.keys[KEY_DOWN]);
		_engine.lua.set_table_table_digital_button("keyboard", "lcontrol", _engine.rain.keys[KEY_LCTRL]);
		_engine.lua.set_table_table_digital_button("keyboard", "rcontrol", _engine.rain.keys[KEY_RCTRL]);
		_engine.lua.set_table_table_digital_button("keyboard", "control", _engine.rain.keys[KEY_CTRL]);
		_engine.lua.set_table_table_digital_button("keyboard", "lshift", _engine.rain.keys[KEY_LSHIFT]);
		_engine.lua.set_table_table_digital_button("keyboard", "rshift", _engine.rain.keys[KEY_RSHIFT]);
		_engine.lua.set_table_table_digital_button("keyboard", "shift", _engine.rain.keys[KEY_SHIFT]);
		_engine.lua.set_table_table_digital_button("keyboard", "alt", _engine.rain.keys[KEY_ALT]);
		_engine.lua.set_table_table_digital_button("keyboard", "caps", _engine.rain.keys[KEY_CAPS]);
		_engine.lua.set_table_table_digital_button("keyboard", "tab", _engine.rain.keys[KEY_TAB]);
		_engine.lua.set_table_table_digital_button("keyboard", "space", _engine.rain.keys[KEY_SPACE]);
		_engine.lua.set_table_table_digital_button("keyboard", "enter", _engine.rain.keys[KEY_RETURN]);
		_engine.lua.set_table_table_digital_button("keyboard", "backspace", _engine.rain.keys[KEY_BACKSPACE]);
		_engine.lua.set_table_table_digital_button("keyboard", "escape", _engine.rain.keys[KEY_ESCAPE]);
		_engine.lua.set_table_table_digital_button("keyboard", "f1", _engine.rain.keys[KEY_F1]);
		_engine.lua.set_table_table_digital_button("keyboard", "f2", _engine.rain.keys[KEY_F2]);
		_engine.lua.set_table_table_digital_button("keyboard", "f3", _engine.rain.keys[KEY_F3]);
		_engine.lua.set_table_table_digital_button("keyboard", "f4", _engine.rain.keys[KEY_F4]);
		_engine.lua.set_table_table_digital_button("keyboard", "f5", _engine.rain.keys[KEY_F5]);
		_engine.lua.set_table_table_digital_button("keyboard", "f6", _engine.rain.keys[KEY_F6]);
		_engine.lua.set_table_table_digital_button("keyboard", "f7", _engine.rain.keys[KEY_F7]);
		_engine.lua.set_table_table_digital_button("keyboard", "f8", _engine.rain.keys[KEY_F8]);
		_engine.lua.set_table_table_digital_button("keyboard", "f9", _engine.rain.keys[KEY_F9]);
		_engine.lua.set_table_table_digital_button("keyboard", "f10", _engine.rain.keys[KEY_F10]);
		_engine.lua.set_table_table_digital_button("keyboard", "f11", _engine.rain.keys[KEY_F11]);
		_engine.lua.set_table_table_digital_button("keyboard", "f12", _engine.rain.keys[KEY_F12]);
	}

	_engine.lua.set_table_number("time", "dt", _engine.rain.dt);
	_engine.lua.set_table_number("time", "seconds", _engine.rain.time_s);
	time_lua_state = GetSeconds() - time_lua_state;
	
	return 0;
}

int lua_swap_buffers(lua_State* l) {
	rain_swap_buffers(&_engine.rain);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	return 0;
}

int lua_sleep(lua_State *l) {
	double t = lua_tonumber(l, 1);
	usleep(t * 1000.0);

	return 0;
}