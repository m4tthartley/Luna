#ifdef _WIN32
	#include <SDL.h>
#elif __APPLE__
	#include <SDL2/SDL.h>
#endif

#include <string>
#include "lua.h"

class Engine {

public:

	SDL_Window *window;
	SDL_GLContext glContext;
	SDL_DisplayMode desktopDisplayMode;
	Lua *lua;

	int width = 1280;
	int height = 720;
	double viewportScale = 1;
	bool fullscreen = false;
	std::string title;
	bool limitFrames = true;
	static int mousex;
	static int mousey;
	static int fps;
	static int tps;
	static int qps;
	static void addqps (int num) { Engine::qps += num; }

	std::string luaRender = "render";
	std::string luaTick = "tick";
	std::string luaKeyDown = "keyDown";
	std::string luaKeyUp = "keyUp";
	std::string luaMouseDown = "mouseDown";

	Engine();
	void run();
	int time();
	void printVersionInfo ();

	static int getMousex(lua_State* l);
	static int getMousey(lua_State* l);
	static int getFrames(lua_State* l);
	static int getTicks(lua_State* l);
	static int getQps(lua_State* l);

};