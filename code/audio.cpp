#include "audio.h"

#include <iostream>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>

void Audio::init() {
	
	alDevice= alcOpenDevice(NULL);
	alContext = alcCreateContext(alDevice, NULL);
	alcMakeContextCurrent(alContext);
	
	ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerVelocity[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVelocity);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

int Audio::loadAudio(std::string file) {
	
	ALuint buffer;
	ALuint source;
	
	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat sourceVelocity[] = { 0.0, 0.0, 0.0 };
	
	Uint32 wavLength;
	Uint8 *wavBuffer;
	SDL_AudioSpec wavSpec;
	
	if(SDL_LoadWAV(file.c_str(), &wavSpec, &wavBuffer, &wavLength) == NULL) {
		std::cout << "AL_ERROR failed to load wav" << std::endl;
		return -1;
	}
	
	alGenBuffers(1, &buffer);
	if(testError(1)) return -1;
#ifdef _WIN32
	ALenum format = AL_FORMAT_MONO16;
#elif __APPLE__
	ALenum format = AL_FORMAT_STEREO16;
#endif
	ALsizei size = wavLength;
	ALvoid *data = wavBuffer;
#ifdef _WIN32
	ALsizei freq = wavSpec.freq;
#elif __APPLE__
	ALsizei freq = wavSpec.freq / 2;
#endif
	ALboolean loop = false;
	
	alBufferData(buffer, format, data, size, freq);
	if(testError(2)) return -1;
	
	alGenSources(1, &source);
	if(testError(3)) return -1;
	
	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 0.5f);
	alSourcefv(source, AL_POSITION, sourcePos);
	alSourcefv(source, AL_VELOCITY, sourceVelocity);
	alSourcei(source, AL_LOOPING, loop);
	if(testError(4)) return -1;
	
	std::cout << file << " was loaded with freq " << freq << std::endl;
	
	return source;
}

void Audio::play(int source) {
	
	alSourcePlay(source);
}

bool Audio::testError(int place) {
	
	switch(alGetError()) {
			
		case AL_INVALID_NAME:
			std::cout << place << " " << "AL_ERROR invalid name" << std::endl;
		break;
		case AL_INVALID_ENUM:
			std::cout << place << " " << "AL_ERROR invalid enum" << std::endl;
		break;
		case AL_INVALID_VALUE:
			std::cout << place << " " << "AL_ERROR invalid value" << std::endl;
		break;
		case AL_INVALID_OPERATION:
			std::cout << place << " " << "AL_ERROR invalid operation" << std::endl;
		break;
		case AL_OUT_OF_MEMORY:
			std::cout << place << " " << "AL_ERROR out of memory" << std::endl;
		break;
	}
	
	return false;
}