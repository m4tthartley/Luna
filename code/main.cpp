
/*
	TODO
	- Check if a file exists locally before curling it
*/

#include "../../libs/rain.c"

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
#include "../liblua/lua.h"
#include "../liblua/lualib.h"
#include "../liblua/lauxlib.h"
}
#endif

#ifdef __APPLE__
#	include "CoreFoundation/CoreFoundation.h"
#	include <curl/curl.h>
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
		res.data = malloc(res.size + 1);
		fread(res.data, 1, res.size, f);
		res.str[res.size] = 0;
		fclose(f);
	}
	return res;
}

bool _local = false;
char *_address = "file:main.lua";
char _path[256];

#ifdef _WIN32
#define far
#include <wininet.h>
#undef far
HINTERNET net_handle = InternetOpen("Luna", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
#endif

char _file_buffer[1024*1024];
int _file_buffer_size = 0;
size_t curl_data_write(void *buffer, size_t size, size_t nmemb, void *user) {
	size_t s = size*nmemb;
	memcpy(_file_buffer + _file_buffer_size, (char*)buffer, s);
//	_file_buffer[_file_buffer_size + size*nmemb] = 0;
	_file_buffer_size += s;
	return s;
}
FileResult load_universal_file(char *file) {
	memset(_file_buffer, 0, _file_buffer_size);
	_file_buffer_size = 0;
	
	char address[256] = {};
	strcpy(address, _path);
	strcat(address, file);
	// printf("%s\n", address);

	bool local_load = _local;
	if (file[0] == 'u' &&
		file[1] == 'r' &&
		file[2] == 'l' &&
		file[3] == ':') {
		file += 4;
		local_load = false;
		address[0] = 0;
		strcpy(address, file);
	}

	if (local_load/*strlen(address) > 5 &&
		address[0] == 'f' &&
		address[1] == 'i' &&
		address[2] == 'l' &&
		address[3] == 'e' &&
		address[4] == ':'*/) {
		// address += 5;
		return load_file(address);
	} else {
#ifdef __APPLE__
		static CURL *c = NULL;
		if (!c) c = curl_easy_init();
		curl_easy_setopt(c, CURLOPT_URL, address);
		curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, curl_data_write);
		CURLcode result = curl_easy_perform(c);
#endif
#ifdef _WIN32
		DWORD context = 567;
		HINTERNET connection = InternetConnect(net_handle, "mattsblog.net", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)&context);
		char *rgpszAcceptTypes[] = {"text/*", "image/*", "font/*", NULL};
		// INTERNET_FLAG_RESYNCHRONIZE
		HINTERNET request = HttpOpenRequestA(connection, NULL, "/particles.lua", NULL, NULL, (LPCSTR*)&rgpszAcceptTypes, 0, (DWORD_PTR)&context);
		//HttpOpenRequest()
		bool result = HttpSendRequest(request, NULL, 0, NULL, 0);
		//char buf
		DWORD len = array_size(_file_buffer);
		DWORD index = 0;
		HttpQueryInfo(request, HTTP_QUERY_ACCEPT, _file_buffer, &len, &index);
		int x = 0;
#endif
		char *mem = (char*)malloc(_file_buffer_size + 1);
		memcpy(mem, _file_buffer, _file_buffer_size);
		mem[_file_buffer_size] = 0;
		return {mem, _file_buffer_size};
	}
}

#include "draw.cpp"
#include "font.cpp"

#include "lua.h"
#include "lua.cpp"

#include "engine.h"
#include "engine.cpp"

//#include "audio.h"

//size_t curl_data_write(void *buffer, size_t size, size_t nmemb, void *user) {
//	printf("size %i, nmemb %i\n", size, nmemb);
//	printf("data\n%s\n", buffer);
//	return size*nmemb;
//}

int main(int argc, char **argv)
//int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
#if 0
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
#endif

	// debug_print("argc %i\n", argc);
	// for (int i = 0; i < argc; ++i) {
	// 	debug_print("arg %s\n", argv[i]);
	// }

//	CURL *c = curl_easy_init();
//	curl_easy_setopt(c, CURLOPT_URL, "mattsblog.net/josh.lua");
//	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, curl_data_write);
//	CURLcode result = curl_easy_perform(c);

	//FileResult file = get_data_from_address("mattsblog.net/josh.lua");
	//printf("%s\n", file.str);
	
#ifdef __APPLE__
	curl_global_init(CURL_GLOBAL_ALL);
#endif

	if (argc > 1) _address = argv[1];
	// if (strlen(_address) > 5 &&
	// 	_address[0] == 'f' &&
	// 	_address[1] == 'i' &&
	// 	_address[2] == 'l' &&
	// 	_address[3] == 'e' &&
	// 	_address[4] == ':') {
	// 	_address += 5;
	// 	_local = true;
	// }

	// if (access(_address, F_OK)) {
	// 	_local = true;
	// }
	FILE *temp_file = fopen(_address, "r");
	if (temp_file) {
		_local = true;
		fclose(temp_file);
	}

	for (int i = strlen(_address); i >= 0; --i) {
		if (_address[i] == '/') {
			memcpy(_path, _address, i+1);
			_path[i+1] = 0;
			_address += i+1;
			break;
		}
	}

	// printf("%s\n%s\n", path, _address);

	printf("\n");
	_engine.run();
	printf("\n");
	
	return 0;
}