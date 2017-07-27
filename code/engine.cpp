
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

static int mousex;
static int mousey;
static int fps;
static int tps;
static int qps;
static void addqps(int num) { qps += num; }

void lua_thread(void *arg) {
	// do {
	// 	if (reload) {
	// 		for (int i = 0; i < texture_count; ++i) {
	// 			glDeleteTextures(1, &textures[i].tex);
	// 		}
	// 		texture_count = 0;
	// 		lua_close(lua.l);
	// 		lua = {};
	// 		lua.init(_address);
	// 		lua.appFunc("init");
	// 		reload = false;
	// 	}
	// 	lua.appFunc("run");
	// } while (reload);

	Lua *lua = (Lua*)arg;
	*lua = {};
	lua->init(_address);
}

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

	void process_luna_event(LunaEvent e) {
		// while (atomic_fetch32(&_event_count) > 0) {
			// LunaEvent *e = &_events[_event_process_index];
			// printf("luna event %i \n", e->type);
			switch (e.type) {
				case EVENT_DRAW_LINE:
					draw_line(e.draw.pos.x, e.draw.pos.y, e.draw.pos2.x, e.draw.pos2.y);
					break;
				case EVENT_DRAW_TRIANGLE:
					draw_triangle(e.draw.pos.x, e.draw.pos.y, e.draw.pos2.x, e.draw.pos2.y, e.draw.pos3.x, e.draw.pos3.y);
					break;
				case EVENT_DRAW_LINE_TRIANGLE:
					draw_line_triangle(e.draw.pos.x, e.draw.pos.y, e.draw.pos2.x, e.draw.pos2.y, e.draw.pos3.x, e.draw.pos3.y);
					break;
				case EVENT_DRAW_RECT:
					draw_rect(e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_DRAW_LINE_RECT:
					draw_line_rect(e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_SET_TEX_COORDS:
					set_tex_coords(e.draw.pos.x, e.draw.pos.y, e.draw.pos2.x, e.draw.pos2.y, e.draw.pos3.x, e.draw.pos3.y, e.draw.pos4.x, e.draw.pos4.y);
					break;
				case EVENT_DRAW_RECT_TEXTURE:
					draw_rect_texture(e.draw.texture, e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_DRAW_CIRCLE:
					draw_circle(e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_DRAW_LINE_CIRCLE:
					draw_line_circle(e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_SET_COLOR:
					glColor4f(e.draw.color.r, e.draw.color.g, e.draw.color.b, e.draw.color.a);
					break;
				case EVENT_LOAD_TEXTURE:
					textures[e.draw.texture].tex = _load_texture(textures[e.draw.texture].file);
					break;
				case EVENT_ROTATE:
					rotate(e.draw.amount);
					break;
				case EVENT_CLEAR_RECT:
					clear_rect(e.draw.pos.x, e.draw.pos.y, e.draw.size.x, e.draw.size.y);
					break;
				case EVENT_CLEAR_COLOR:
					_clear_color = {e.draw.color.r, e.draw.color.g, e.draw.color.b, e.draw.color.a};
					break;
				case EVENT_PRESENT:
					rain_swap_buffers(&rain);
					break;
			}

			
		// }
	}

	void run() {
		// lua.init(_address);
		// lua.appFunc("init");
		/*if (lua.get_table_table_var("window", "size", "x")) rain.window_width = lua_tonumber(lua.l, -1);
		if (lua.get_table_table_var("window", "size", "y")) rain.window_height = lua_tonumber(lua.l, -1);*/

		// rain.window_title = lua.get_table_var("window", "title");

		rain.multisample_window = true;
		rain_init(&rain);

		glLineWidth(2.0f);

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

		SDL_LunaEvent = SDL_RegisterEvents(1);
		if (SDL_LunaEvent == 0xFFFFFFFF) {
			printf("SDL_RegisterEvents failed! \n");
		}

		create_thread(lua_thread, &lua);

		// glViewport(0, 0, rain.window_width, rain.window_height);
		// glMatrixMode(GL_PROJECTION);
		// glLoadIdentity();
		// glOrtho(0, rain.window_width, rain.window_height, 0, -100, 100);
		// glMatrixMode(GL_MODELVIEW);
		// glLoadIdentity();

		while (!rain.quit) {
			rain.mouse.position_delta.x = 0;
			rain.mouse.position_delta.y = 0;
			rain.mouse.wheel_delta = 0;
			
			rain.mouse.left.pressed = false;
			rain.mouse.left.released = false;
			rain.mouse.right.pressed = false;
			rain.mouse.right.released = false;
			rain.mouse.middle.pressed = false;
			rain.mouse.middle.released = false;
			
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						rain.quit = true;
						break;
					{case SDL_MOUSEBUTTONDOWN:
						LunaEvent e = {};
						e.type = EVENT_MOUSE_DOWN;
						if (event.button.button == SDL_BUTTON_LEFT) {
							update_digital_button(&rain.mouse.left, true);
							e.input.mouse_button = 0;
						}
						if (event.button.button == SDL_BUTTON_RIGHT) {
							update_digital_button(&rain.mouse.right, true);
							e.input.mouse_button = 1;
						}
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							update_digital_button(&rain.mouse.middle, true);
							e.input.mouse_button = 2;
						}
						input_queue.push_event(e);
						break;}
					{case SDL_MOUSEBUTTONUP:
						LunaEvent e = {};
						e.type = EVENT_MOUSE_UP;
						if (event.button.button == SDL_BUTTON_LEFT) {
							update_digital_button(&rain.mouse.left, false);
							e.input.mouse_button = 0;
						}
						if (event.button.button == SDL_BUTTON_RIGHT) {
							update_digital_button(&rain.mouse.right, false);
							e.input.mouse_button = 1;
						}
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							update_digital_button(&rain.mouse.middle, false);
							e.input.mouse_button = 2;
						}
						input_queue.push_event(e);
						break;}
					{case SDL_MOUSEMOTION:
						// LunaEvent e = {};
						// e.type = EVENT_MOUSE_MOTION;
						rain.mouse.position = {event.motion.x, event.motion.y};
						rain.mouse.position_delta = {event.motion.xrel, event.motion.yrel};
						// e.input.mouse_position = rain.mouse.position;
						// input_queue.push_event(e);
						break;}
					{case SDL_MOUSEWHEEL:
						LunaEvent e = {};
						e.type = EVENT_MOUSE_MOTION;
						rain.mouse.wheel_delta += event.wheel.y;
						e.input.amount = event.wheel.y;
						input_queue.push_event(e);
						break;}
					{case SDL_WINDOWEVENT:
						switch (event.window.event) {
							case SDL_WINDOWEVENT_RESIZED:
								LunaEvent e = {};
								e.type = EVENT_WINDOW_RESIZE;
								rain.window_width = event.window.data1;
								rain.window_height = event.window.data2;
								e.input.window_size = {rain.window_width, rain.window_height};
								input_queue.push_event(e);
								break;
						}
						break;}
				}

				if (event.type == SDL_LunaEvent) {
					LunaEvent e = command_queue.pull_event();
					process_luna_event(e);
					// printf("event %s\n", event_type_names[e.type]);
				}
			}
			
			int numkeys;
			const unsigned char *keys = SDL_GetKeyboardState(&numkeys);
			for (int i = 0; i < 256; ++i) {
				// int keycode = SDL_GetKeyFromScancode(i);
				update_digital_button(&rain.keys[i], keys[i]);
			}

			// if (rain.keys[SDL_GetScancodeFromKey('W')].pressed) printf("KEY PRESSED\n");

			rain_poll_time(&rain);

			if (rain.quit) {
				// Force the lua run loop to quit
				// lua_pushstring(_engine.lua.l, "Quitting program...");
				// lua_error(_engine.lua.l);
			}

			static bool reload_shortcut = false;
		#ifdef _WIN32
			if (GetAsyncKeyState(VK_CONTROL) && (GetAsyncKeyState('S') || GetAsyncKeyState('R'))) {
				if (!reload_shortcut) {
					/*for (int i = 0; i < texture_count; ++i) {
						glDeleteTextures(1, &textures[i].tex);
					}
					texture_count = 0;
					lua_close(lua.l);
					lua = Lua(default_lua_file);
					lua.appFunc("init");*/

					_engine.reload = true;
					// lua_pushstring(_engine.lua.l, "reloading...");
					// lua_error(_engine.lua.l);
				}
				reload_shortcut = true;
			} else {
				reload_shortcut = false;
			}
		#endif
		#ifdef __APPLE__
			if ((rain.keys[KEY_CTRL].down || rain.keys[KEY_LGUI].down) && (rain.keys[KEY_S].pressed || rain.keys[KEY_R].pressed)) {
				if (!reload_shortcut) {
					// for (int i = 0; i < texture_count; ++i) {
					// 	glDeleteTextures(1, &textures[i].tex);
					// }
					// texture_count = 0;
					// lua_close(lua.l);
					// lua = {};
					// lua.init(address);
					// lua.appFunc("init");
					reload = true;
					// lua_pushstring(_engine.lua.l, "reloading...");
					// lua_error(_engine.lua.l);
				}
				reload_shortcut = true;
			} else {
				reload_shortcut = false;
			}
		#endif

			// rain_swap_buffers(&rain);
		}

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
