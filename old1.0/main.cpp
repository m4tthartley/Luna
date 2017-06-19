#include "engine.h"
#include "audio.h"

#include <iostream>

#ifdef __APPLE__
	#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char **argv) {
	
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
	
	Engine *engine = new Engine();
	engine->run();
	
	delete engine;
	return 1;
}