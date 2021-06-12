#include "xpch.h"
#include "Engine/Debug/Logger.h"
#include "Audio.h"
#include "../../Dependencies/sndfile/build/include/sndfile.h"
//#include <sndfile.h>
#include <inttypes.h>

#define ALC_API
#define AL_API
#include <AL\alext.h>

#define OpenAL_ErrorCheck(message) \
{\
	ALenum error = alGetError();\
	if (error != ALC_NO_ERROR)\
	 {\
		std::cout << "OpenAL error: " << error << " for " << #message << std::endl;\
   }\
}


std::unordered_map<std::string, SoundBuffer*>* Audio::s_SoundBuffers = nullptr;
void Audio::Init()
{
	const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
ALCdevice* device = alcOpenDevice(defaultDeviceString);
if (!device)
{
	std::cout << "ERROR: OpenAL" << std::endl;
}

ALCcontext* context = alcCreateContext(device, nullptr);

if (!alcMakeContextCurrent(context))
{
	std::cout << "Could not make context!" << std::endl;
}

OpenAL_ErrorCheck("Make Context Current");

s_SoundBuffers = new std::unordered_map<std::string,SoundBuffer*>();
}

void Audio::Close()
{
	for (auto it = s_SoundBuffers->begin(); it != s_SoundBuffers->end(); it++)
	{
		delete (*it).second;
	}
	delete s_SoundBuffers;
}

void Audio::Load(const std::string& file)
{
	auto it = s_SoundBuffers->find(file.c_str());
	if (it != s_SoundBuffers->end())
	{
		//it is already in the map
		Log_EngineWarn("Trying to load texture that is already loaded: {0}",file);
		return;
	}

	SNDFILE* sndfile;
	SF_INFO sfinfo;
	sndfile = sf_open(file.c_str(), SFM_READ, &sfinfo);
	if (!sndfile)
	{
		Log_EngineError("Could not open audi file: {0}",file);
		return;
	}

	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		Log_EngineError("Audio file \"{0}\" has bad sample count: {1}", file, sfinfo.frames);
		sf_close(sndfile);
		return;
	}

	ALenum format = AL_NONE;
	switch (sfinfo.channels)
	{
	case 1:
		format = AL_FORMAT_MONO16;
		break;
	case 2: 
		format = AL_FORMAT_STEREO16;
		break;
	case 3:
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
		break;
	case 4:
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
		break;
	default: 
		break;
	}

	if (!format)
	{
		Log_EngineError("File \"{0}\" has unsupported channel count: {1}",file, sfinfo.channels);
		sf_close(sndfile);
		return;
	}

	short* membuffer = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	sf_count_t num_frames = sf_readf_short(sndfile, membuffer, sfinfo.frames);

	if (num_frames < 1)
	{
		free(membuffer);
		sf_close(sndfile);
		Log_EngineError("Failed to read samples from file: {0}", file);
		return;
	}

	ALsizei num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	SoundBuffer* result = new SoundBuffer(format, membuffer, num_bytes, sfinfo.samplerate);
	sf_close(sndfile);

	free(membuffer);
	//std::cout << file.c_str() << std::endl;
	(*s_SoundBuffers)[file] = result;	
	//std::cout << s_SoundBuffers->size() << std::endl;
}

SoundBuffer* Audio::Get(const std::string& file)
{
	auto it = s_SoundBuffers->find(file.c_str());
	if (it == s_SoundBuffers->end())
	{
		Log_EngineWarn("Tried to get an audio file that was never loaded: {0}",file);
		return nullptr;
	}
	return (*s_SoundBuffers)[file];
}
