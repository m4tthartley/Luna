
#include "w:/libs/rain.c"

#include "types.h"

//#ifdef _WIN32
//#include "../sdl/include/SDL.h"
//#undef main
//#elif __APPLE__
//#include <SDL2/SDL.h>
//#endif

#include <string>

//#include "engine.h"
//#include "audio.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <string>
#include <functional>
#include <map>

#ifdef _WIN32
//#include <glew.h>
#include <windows.h>
#include <GL/gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#endif

// #include "video3.h"
// #include "shader.h"

#ifdef _WIN32
#include "../lua/include/lua.hpp"
#elif __APPLE__
extern "C" {
#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>
}
#endif

#include "video.h"
#include "video.cpp"
#include "videoLua.h"
#include "videoLua.cpp"

struct FileResult {
	union {
		void *data;
		char *str;
	};
	int64 size;
};
FileResult load_file(char *file) {
	FileResult res = {};
	FILE *f = fopen(file, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		res.size = ftell(f);
		fseek(f, 0, SEEK_SET);
		res.data = malloc(res.size);
		fread(res.data, 1, res.size, f);
		fclose(f);
	}
	return res;
}

#include "draw.cpp"
#include "font.cpp"

#include "lua.h"
#include "lua.cpp"

#include "engine.h"
#include "engine.cpp"

//#include "audio.h"

#ifdef __APPLE__
	#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char **argv)
//int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{	
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);
	
	chdir(path);
	chdir("../../..");
	//std::cout << "Current Path: " << path << std::endl;
#endif

	Engine engine = Engine();
	engine.run();
	
	return 0;
}