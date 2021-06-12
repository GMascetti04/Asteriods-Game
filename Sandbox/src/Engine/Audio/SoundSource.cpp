#include "xpch.h"
#include "SoundSource.h"
#define ALC_API
#define AL_API
#include "AL/al.h"
#include "AL/alc.h"

SoundSource::SoundSource()
{
	alGenSources(1, &m_id);
	alSourcef(m_id, AL_GAIN, 1);
	alSourcef(m_id, AL_PITCH, 1);
	alSource3f(m_id, AL_POSITION, 0, 0, 0);
	m_currentSoundBuffer = nullptr;
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &m_id);
}

void SoundSource::AttachBuffer(SoundBuffer* buffer)
{
	m_currentSoundBuffer = buffer;
	alSourcei(m_id, AL_BUFFER, buffer->getID());
	alSourcef(m_id, AL_GAIN, 0.2f);
}

void SoundSource::playSound()
{
	alSourcePlay(m_id);
}

void SoundSource::pauseSound()
{
	alSourcePause(m_id);
}

void SoundSource::stopSound()
{
	alSourceStop(m_id);
}

void SoundSource::setLoop(bool loop)
{
	alSourcei(m_id, AL_LOOPING, (int)loop);
}
