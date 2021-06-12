#include "xpch.h"
#include "SoundBuffer.h"

#define ALC_API
#define AL_API
#include <AL\al.h>
#include <AL\alc.h>

SoundBuffer::SoundBuffer(int format, void* data, int size, int frequency) : m_id(0), m_format(format),
 m_data(nullptr), m_size(size), m_frequency(frequency)
{

	m_data = new short[size / sizeof(short)];
	memcpy(m_data, data, size);
	alGenBuffers(1, &m_id);
	alBufferData(m_id, m_format, m_data, m_size, m_frequency);
	
	

}

//fix this shit later
SoundBuffer::SoundBuffer()
{
}

SoundBuffer::SoundBuffer(const SoundBuffer& oldBuffer)
{

}



SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &m_id);
	delete[] m_data;
}
