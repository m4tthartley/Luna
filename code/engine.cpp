
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_MULTISAMPLE                    0x809D

static int mousex;
static int mousey;
static int fps;
static int tps;
static int qps;
static void addqps(int num) { qps += num; }

void lua_thread_proc(void *arg) {
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

	Thread lua_thread;

	/*Engine();
	void run();
	int time();
	void printVersionInfo();

	static int getMousex(lua_State* l);
	static int getMousey(lua_State* l);
	static int getFrames(lua_State* l);
	static int getTicks(lua_State* l);
	static int getQps(lua_State* l);*/

	SDL_Window *sdl_window;

	void process_luna_event(LunaEvent e) {
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
				//rain_swap_buffers(&rain);
				SDL_GL_SwapWindow(sdl_window);
				break;

			case EVENT_LOAD_FONT:
				LoadFont(e.draw.file, e.draw.scale);
				break;
			case EVENT_DRAW_FONT:
				draw_font(e.draw.file, e.draw.scale, e.draw.str, {e.draw.pos.x, e.draw.pos.y, 0}, e.draw.size.x);
				break;
		}
	}

	void run() {
		// lua.init(_address);
		// lua.appFunc("init");
		/*if (lua.get_table_table_var("window", "size", "x")) rain.window_width = lua_tonumber(lua.l, -1);
		if (lua.get_table_table_var("window", "size", "y")) rain.window_height = lua_tonumber(lua.l, -1);*/

		// rain.window_title = lua.get_table_var("window", "title");

		rain.multisample_window = true;
		// rain_init(&rain);
		{
			if (!rain.window_width || !rain.window_height) {
				rain.window_width = 1280;
				rain.window_height = 720;
			}
			if (!rain.window_title) {
				rain.window_title = "Rain";
			}

			load_sdl_procs();

			SDL_SetMainReady();
			SDL_Init(SDL_INIT_VIDEO);

			if (rain.multisample_window) {
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
			}

			sdl_window = SDL_CreateWindow(rain.window_title,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				rain.window_width, rain.window_height,
				SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

			/*SDL_GLContext gl_context =*/ SDL_GL_CreateContext(sdl_window);

			if (rain.multisample_window) {
				glEnable(GL_MULTISAMPLE);
			}

			rain.start_time = GetTime();
			rain.old_time = rain.start_time;
		}

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
		SDL_GL_SwapWindow(sdl_window);

		init_font_system();

		SDL_LunaEvent = SDL_RegisterEvents(1);
		if (SDL_LunaEvent == 0xFFFFFFFF) {
			printf("SDL_RegisterEvents failed! \n");
		}

		LoadFont(DEBUG_FONT, 1.0f);

		lua_thread = create_thread(lua_thread_proc, &lua);

		int num_luna_events_we_got = 0;
		int num_frames = 0;

		SDL_GL_SetSwapInterval(0);

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
			SDL_WaitEvent(&event);
			do {
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
					{case SDL_KEYDOWN:
						if ((rain.keys[SDL_SCANCODE_LCTRL].down || rain.keys[SDL_SCANCODE_LGUI].down) && event.key.keysym.scancode == SDL_SCANCODE_R) {
							destroy_thread(lua_thread);
							for (int i = 0; i < texture_count; ++i) {
								glDeleteTextures(1, &textures[i].tex);
							}
							texture_count = 0;
							lua_close(lua.l);
							lua = {};
							lua_thread = create_thread(lua_thread_proc, &lua);
						}
						break;}
				}

				if (event.type == SDL_LunaEvent) {
					// LunaEvent e = command_queue.pull_event();
					// process_luna_event(e);
				}
			} while (SDL_PollEvent(&event));

			// printf("command events %i \n", atomic_fetch32(&command_queue.count));

			atomic_swap32(&command_queue.window_event, false);

			// LunaEvent e;
			// while ((e = command_queue.pull_event()).type != EVENT_NONE)
			int event_count = atomic_fetch32(&command_queue.count);
			// printf("events %i\n", event_count);
			for (int i = 0; i < event_count; ++i) {
				LunaEvent e = command_queue.pull_event();
				process_luna_event(e);
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

					reload = true;
					// lua_pushstring(_engine.lua.l, "reloading...");
					// lua_error(_engine.lua.l);
				}
				reload_shortcut = true;
			} else {
				reload_shortcut = false;
			}
		#endif
		#ifdef __APPLE__
			#if 0
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

					destroy_thread(lua_thread);
					for (int i = 0; i < texture_count; ++i) {
						glDeleteTextures(1, &textures[i].tex);
					}
					texture_count = 0;
					lua_close(lua.l);
					lua = {};
					lua_thread = create_thread(lua_thread_proc, &lua);
				}
				reload_shortcut = true;
			} else {
				reload_shortcut = false;
			}
			#endif
		#endif

			// rain_swap_buffers(&rain);

			// ++num_frames;
			// printf("frame %i\n", num_frames);
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
