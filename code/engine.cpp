
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
	Lua *lua;

	int width = 1280;
	int height = 720;
	double viewportScale = 1;
	bool fullscreen = false;
	std::string title;
	bool limitFrames = true;
	
	std::string luaRender = "render";
	std::string luaTick = "tick";
	std::string luaKeyDown = "keyDown";
	std::string luaKeyUp = "keyUp";
	std::string luaMouseDown = "mouseDown";

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

	Engine() {
		lua = new Lua();
		lua->appFunc("init");
		if (lua->get_table_table_var("window", "size", "x")) rain.window_width = lua_tonumber(lua->l, -1);
		if (lua->get_table_table_var("window", "size", "y")) rain.window_height = lua_tonumber(lua->l, -1);
		if (lua->get_table_var("window", "title")) rain.window_title = (char*)lua_tostring(lua->l, -1);

		rain_init(&rain);

		enable_dynamic_texture_loading = true;
		for (int i = 0; i < texture_count; ++i) {
			textures[i].tex = _load_texture(textures[i].file);
		}
		
		//lua->loadSettings(width, height, viewportScale, fullscreen, title, limitFrames);

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

		while (!rain.quit) {
			rain_update(&rain);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			lua->set_table_table_number("mouse", "position", "x", rain.mouse.position.x);
			lua->set_table_table_number("mouse", "position", "y", rain.mouse.position.y);
			lua->set_table_table_number("mouse", "position_delta", "x", rain.mouse.position_delta.x);
			lua->set_table_table_number("mouse", "position_delta", "y", rain.mouse.position_delta.y);

			lua->set_table_table_bool("mouse", "left", "down", rain.mouse.left.down);
			lua->set_table_table_bool("mouse", "left", "pressed", rain.mouse.left.pressed);
			lua->set_table_table_bool("mouse", "left", "released", rain.mouse.left.released);

			lua->set_table_table_bool("mouse", "right", "down", rain.mouse.right.down);
			lua->set_table_table_bool("mouse", "right", "pressed", rain.mouse.right.pressed);
			lua->set_table_table_bool("mouse", "right", "released", rain.mouse.right.released);

			lua->set_table_table_bool("mouse", "middle", "down", rain.mouse.middle.down);
			lua->set_table_table_bool("mouse", "middle", "pressed", rain.mouse.middle.pressed);
			lua->set_table_table_bool("mouse", "middle", "released", rain.mouse.middle.released);

			lua->set_table_number("mouse", "wheel_delta", rain.mouse.wheel_delta);

			{
				lua->set_table_table_digital_button("keyboard", "n1", rain.keys[KEY_1]);
				lua->set_table_table_digital_button("keyboard", "n2", rain.keys[KEY_2]);
				lua->set_table_table_digital_button("keyboard", "n3", rain.keys[KEY_3]);
				lua->set_table_table_digital_button("keyboard", "n4", rain.keys[KEY_4]);
				lua->set_table_table_digital_button("keyboard", "n5", rain.keys[KEY_5]);
				lua->set_table_table_digital_button("keyboard", "n6", rain.keys[KEY_6]);
				lua->set_table_table_digital_button("keyboard", "n7", rain.keys[KEY_7]);
				lua->set_table_table_digital_button("keyboard", "n8", rain.keys[KEY_8]);
				lua->set_table_table_digital_button("keyboard", "n9", rain.keys[KEY_9]);
				lua->set_table_table_digital_button("keyboard", "n0", rain.keys[KEY_0]);
				lua->set_table_table_digital_button("keyboard", "a", rain.keys[KEY_A]);
				lua->set_table_table_digital_button("keyboard", "b", rain.keys[KEY_B]);
				lua->set_table_table_digital_button("keyboard", "c", rain.keys[KEY_C]);
				lua->set_table_table_digital_button("keyboard", "d", rain.keys[KEY_D]);
				lua->set_table_table_digital_button("keyboard", "e", rain.keys[KEY_E]);
				lua->set_table_table_digital_button("keyboard", "f", rain.keys[KEY_F]);
				lua->set_table_table_digital_button("keyboard", "g", rain.keys[KEY_G]);
				lua->set_table_table_digital_button("keyboard", "h", rain.keys[KEY_H]);
				lua->set_table_table_digital_button("keyboard", "i", rain.keys[KEY_I]);
				lua->set_table_table_digital_button("keyboard", "j", rain.keys[KEY_J]);
				lua->set_table_table_digital_button("keyboard", "k", rain.keys[KEY_K]);
				lua->set_table_table_digital_button("keyboard", "l", rain.keys[KEY_L]);
				lua->set_table_table_digital_button("keyboard", "m", rain.keys[KEY_M]);
				lua->set_table_table_digital_button("keyboard", "n", rain.keys[KEY_N]);
				lua->set_table_table_digital_button("keyboard", "o", rain.keys[KEY_O]);
				lua->set_table_table_digital_button("keyboard", "p", rain.keys[KEY_P]);
				lua->set_table_table_digital_button("keyboard", "q", rain.keys[KEY_Q]);
				lua->set_table_table_digital_button("keyboard", "r", rain.keys[KEY_R]);
				lua->set_table_table_digital_button("keyboard", "s", rain.keys[KEY_S]);
				lua->set_table_table_digital_button("keyboard", "t", rain.keys[KEY_T]);
				lua->set_table_table_digital_button("keyboard", "u", rain.keys[KEY_U]);
				lua->set_table_table_digital_button("keyboard", "v", rain.keys[KEY_V]);
				lua->set_table_table_digital_button("keyboard", "w", rain.keys[KEY_W]);
				lua->set_table_table_digital_button("keyboard", "x", rain.keys[KEY_X]);
				lua->set_table_table_digital_button("keyboard", "y", rain.keys[KEY_Y]);
				lua->set_table_table_digital_button("keyboard", "z", rain.keys[KEY_Z]);

				lua->set_table_table_digital_button("keyboard", "left", rain.keys[KEY_LEFT]);
				lua->set_table_table_digital_button("keyboard", "right", rain.keys[KEY_RIGHT]);
				lua->set_table_table_digital_button("keyboard", "up", rain.keys[KEY_UP]);
				lua->set_table_table_digital_button("keyboard", "down", rain.keys[KEY_DOWN]);
				lua->set_table_table_digital_button("keyboard", "lcontrol", rain.keys[KEY_LCTRL]);
				lua->set_table_table_digital_button("keyboard", "rcontrol", rain.keys[KEY_RCTRL]);
				lua->set_table_table_digital_button("keyboard", "control", rain.keys[KEY_CTRL]);
				lua->set_table_table_digital_button("keyboard", "lshift", rain.keys[KEY_LSHIFT]);
				lua->set_table_table_digital_button("keyboard", "rshift", rain.keys[KEY_RSHIFT]);
				lua->set_table_table_digital_button("keyboard", "shift", rain.keys[KEY_SHIFT]);
				lua->set_table_table_digital_button("keyboard", "alt", rain.keys[KEY_ALT]);
				lua->set_table_table_digital_button("keyboard", "caps", rain.keys[KEY_CAPS]);
				lua->set_table_table_digital_button("keyboard", "tab", rain.keys[KEY_TAB]);
				lua->set_table_table_digital_button("keyboard", "space", rain.keys[KEY_SPACE]);
				lua->set_table_table_digital_button("keyboard", "enter", rain.keys[KEY_RETURN]);
				lua->set_table_table_digital_button("keyboard", "backspace", rain.keys[KEY_BACKSPACE]);
				lua->set_table_table_digital_button("keyboard", "escape", rain.keys[KEY_ESCAPE]);
				lua->set_table_table_digital_button("keyboard", "f1", rain.keys[KEY_F1]);
				lua->set_table_table_digital_button("keyboard", "f2", rain.keys[KEY_F2]);
				lua->set_table_table_digital_button("keyboard", "f3", rain.keys[KEY_F3]);
				lua->set_table_table_digital_button("keyboard", "f4", rain.keys[KEY_F4]);
				lua->set_table_table_digital_button("keyboard", "f5", rain.keys[KEY_F5]);
				lua->set_table_table_digital_button("keyboard", "f6", rain.keys[KEY_F6]);
				lua->set_table_table_digital_button("keyboard", "f7", rain.keys[KEY_F7]);
				lua->set_table_table_digital_button("keyboard", "f8", rain.keys[KEY_F8]);
				lua->set_table_table_digital_button("keyboard", "f9", rain.keys[KEY_F9]);
				lua->set_table_table_digital_button("keyboard", "f10", rain.keys[KEY_F10]);
				lua->set_table_table_digital_button("keyboard", "f11", rain.keys[KEY_F11]);
				lua->set_table_table_digital_button("keyboard", "f12", rain.keys[KEY_F12]);
			}

			lua->set_table_number("time", "dt", rain.dt);
			lua->set_table_number("time", "seconds", rain.time_s);

			lua->appFunc("update");
		}
	}

	void run() {

	}

#if 0
	void printVersionInfo() {

		SDL_version sdlVersion;
		SDL_GetVersion(&sdlVersion);
		std::cout << "sdl version(" << std::to_string(sdlVersion.major) <<
			" " << std::to_string(sdlVersion.minor) << ")" << std::endl;
		std::cout << "gl(" << glGetString(GL_VERSION) << ") glsl(" << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << std::endl;
	}

	Engine() {

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cout << SDL_GetError() << std::endl;
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if (window == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			return;
		}

		// Audio::init();

		glContext = SDL_GL_CreateContext(window);
		//printVersionInfo();

#ifdef _WIN32
		//glewInit();
#endif
		//std::cout << "glsl " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		//	int majorVersion;
		//	int minorVersion;
		//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
		//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);
		//	std::cout << majorVersion << " " << minorVersion << std::endl;

		//Video2::init();
		//Video3::init();
		//Shader::compileDefault();

		lua = new Lua();
		lua->loadSettings(width, height, viewportScale, fullscreen, title, limitFrames);
	}

	void run() {
		SDL_SetMainReady();

		SDL_SetWindowTitle(window, title.c_str());
		SDL_SetWindowSize(window, (int)(width*viewportScale), (int)(height*viewportScale));
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		if (fullscreen) SDL_SetWindowFullscreen(window, SDL_TRUE);
		SDL_GetDesktopDisplayMode(0, &desktopDisplayMode);
		//std::cout << desktopDisplayMode.w << " " << desktopDisplayMode.h << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);

		glAlphaFunc(GL_GREATER, 0.1f);
		glEnable(GL_ALPHA_TEST);

		bool quit = false;
		SDL_Event e;

		glViewport(0, 0, (int)(width*viewportScale), (int)(height*viewportScale));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1);
		if (!limitFrames) SDL_GL_SetSwapInterval(0);

		double time = this->time();
		double tickTime = this->time();
		int frames = 0;
		int ticks = 0;
		double msPerTick = (double)1000.0 / 60.0;
		double unprocessed = 0.0;

		while (!quit) {

			while (SDL_PollEvent(&e)) {

				if (e.type == SDL_QUIT) quit = true;

				if (e.type == SDL_KEYDOWN) {
					lua->appFunc(luaKeyDown, e.key.keysym.scancode);

					if (e.key.keysym.scancode == SDL_SCANCODE_F11) {
						if (!fullscreen) {
							fullscreen = true;
							//SDL_SetWindowSize(window, desktopDisplayMode.w, desktopDisplayMode.h);
							if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) std::cout << SDL_GetError();
							glViewport(0, 0, desktopDisplayMode.w, desktopDisplayMode.h);
						} else if (fullscreen) {
							fullscreen = false;
							if (SDL_SetWindowFullscreen(window, 0) != 0) std::cout << SDL_GetError();
							glViewport(0, 0, (int)(width*viewportScale), (int)(height*viewportScale));
							//SDL_SetWindowSize(window, width*viewportScale, height*viewportScale);
						}
					}
				}

				if (e.type == SDL_KEYUP) lua->appFunc(luaKeyUp, e.key.keysym.scancode);

				if (e.type == SDL_MOUSEBUTTONDOWN) {
					if (e.button.button == SDL_BUTTON_LEFT) lua->appFunc(luaMouseDown, 0);
					if (e.button.button == SDL_BUTTON_RIGHT) lua->appFunc(luaMouseDown, 1);
				}

				if (e.type == SDL_MOUSEMOTION) {
					mousex = e.motion.x;
					mousey = e.motion.y;
				}
			}

			if (!lua->error) {

				/*if (this->time() - time > 1000.0) {
					fps = frames;
					tps = ticks;
					frames = 0;
					ticks = 0;
					time = this->time();
					lua->appFunc("secondstep");
				}

				unprocessed += (this->time() - tickTime) / msPerTick;
				tickTime = this->time();

				while (unprocessed >= 1.0) {
					unprocessed--;
					lua->appFunc(luaTick);
					ticks++;
				}*/

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				lua->appFunc(luaRender);
				frames++;
				SDL_GL_SwapWindow(window);
				qps = 0;
			}
		}

		SDL_DestroyWindow(window);
		SDL_Quit();
	}
#endif

	/*int time() {

		return SDL_GetTicks();
	}*/

	int getMousex(lua_State* l) {

		lua_pushnumber(l, mousex);
		return 1;
	}

	int getMousey(lua_State* l) {

		lua_pushnumber(l, mousey);
		return 1;
	}

	int getFrames(lua_State* l) {

		lua_pushnumber(l, fps);
		return 1;
	}

	int getTicks(lua_State* l) {

		lua_pushnumber(l, tps);
		return 1;
	}

	int getQps(lua_State *l) {

		lua_pushnumber(l, qps);
		return 1;
	}

};
