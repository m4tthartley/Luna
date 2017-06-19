#ifdef _WIN32
	#include <SDL.h>
#elif __APPLE__
	#include <SDL2/SDL.h>
#endif

#include <string>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>

static ALCdevice *alDevice;
static ALCcontext *alContext;

class Audio {
	
public:
	
	static void init();
	static int loadAudio(std::string file);
	static void play(int source);
	static bool testError(int place);
	
};