#pragma once
#include "Engine/Utilities/Vec.h"
#include "Engine/Audio/SoundBuffer.h"
class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void AttachBuffer(SoundBuffer* buffer);
	void playSound();
	void pauseSound();
	void stopSound();

	void setLoop(bool loop);

	SoundBuffer* getSoundBuffer() { return m_currentSoundBuffer; }
private:
	unsigned int m_id;

	SoundBuffer* m_currentSoundBuffer;
};

