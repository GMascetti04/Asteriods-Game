#pragma once
#include "xpch.h"
#include "SoundBuffer.h"
class Audio
{
public:
	Audio() = delete;
	~Audio() = delete;

	static void Init();
	static void Close();

	static void Load(const std::string& file);

	static SoundBuffer* Get(const std::string& file);

private:

	static std::unordered_map<std::string, SoundBuffer*>* s_SoundBuffers;
};

