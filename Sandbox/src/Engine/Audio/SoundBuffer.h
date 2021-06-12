#pragma once
class SoundBuffer
{
public:
	SoundBuffer(int format, void* data, int size, int frequency);
	SoundBuffer();

	SoundBuffer(const SoundBuffer& oldBuffer);
	
	
	~SoundBuffer();

	unsigned int getID() { return m_id; }
private:
	unsigned int m_id;

	int m_format;
	void* m_data;
	int m_size;
	int m_frequency;
};

