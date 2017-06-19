#include "engine.h"
#include "audio.h"
#include <iostream>
#include <ctime>

#ifdef _WIN32
	#include <glew.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
#endif

#include "video3.h"
#include "shader.h"

int Engine::mousex = 0;
int Engine::mousey = 0;
int Engine::fps = 0;
int Engine::tps = 0;
int Engine::qps = 0;

void Engine::printVersionInfo () {
	
	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);
	std::cout << "sdl version(" << std::to_string(sdlVersion.major) <<
	" " << std::to_string(sdlVersion.minor) << ")" << std::endl;
	std::cout << "gl(" << glGetString(GL_VERSION) << ") glsl(" << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << std::endl;
}

Engine::Engine() {

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
	
	Audio::init();

	glContext = SDL_GL_CreateContext(window);
	//printVersionInfo();
	
#ifdef _WIN32
	glewInit();
#endif
	//std::cout << "glsl " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
//	int majorVersion;
//	int minorVersion;
//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);
//	std::cout << majorVersion << " " << minorVersion << std::endl;
	
	//Video2::init();
	Video3::init();
	//Shader::compileDefault();
	
	lua = new Lua();
	lua->loadSettings(width, height, viewportScale, fullscreen, title, limitFrames);
}

void Engine::run() {
	
	SDL_SetWindowTitle(window, title.c_str());
	SDL_SetWindowSize(window, width*viewportScale, height*viewportScale);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	if (fullscreen) SDL_SetWindowFullscreen(window, SDL_TRUE);
	SDL_GetDesktopDisplayMode(0, &desktopDisplayMode);
	//std::cout << desktopDisplayMode.w << " " << desktopDisplayMode.h << std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	
	bool quit = false;
	SDL_Event e;

	glViewport(0, 0, width*viewportScale, height*viewportScale);
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
				
				if(e.key.keysym.scancode == SDL_SCANCODE_F11) {
					if(!fullscreen) {
						fullscreen = true;
						//SDL_SetWindowSize(window, desktopDisplayMode.w, desktopDisplayMode.h);
						if(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) std::cout << SDL_GetError();
						glViewport(0, 0, desktopDisplayMode.w, desktopDisplayMode.h);
					}
					else if(fullscreen) {
						fullscreen = false;
						if(SDL_SetWindowFullscreen(window, 0) != 0) std::cout << SDL_GetError();
						glViewport(0, 0, width*viewportScale, height*viewportScale);
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

			if (this->time() - time > 1000.0) {
				fps = frames;
				tps = ticks;
				frames = 0;
				ticks = 0;
				time = this->time();
				lua->appFunc("secondstep");
				//std::cout << this->time() << " " << CLOCKS_PER_SEC << std::endl;
			}

			unprocessed += (this->time() - tickTime) / msPerTick;
			tickTime = this->time();

			while (unprocessed >= 1.0) {
				unprocessed--;
				lua->appFunc(luaTick);
				ticks++;
			}

			lua->appFunc(luaRender);
			frames++;
			SDL_GL_SwapWindow(window);
			qps = 0;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Engine::time() {
	
	return SDL_GetTicks();
}

int Engine::getMousex(lua_State* l) {

	lua_pushnumber(l, mousex);
	return 1;
}

int Engine::getMousey(lua_State* l) {

	lua_pushnumber(l, mousey);
	return 1;
}

int Engine::getFrames(lua_State* l) {

	lua_pushnumber(l, fps);
	return 1;
}

int Engine::getTicks(lua_State* l) {

	lua_pushnumber(l, tps);
	return 1;
}

int Engine::getQps (lua_State *l) {
	
	lua_pushnumber(l, qps);
	return 1;
}