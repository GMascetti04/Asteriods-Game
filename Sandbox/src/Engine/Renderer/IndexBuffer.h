#pragma once
#include "Engine/Debug/Profiler.h"

/*
* The IndexBuffer class is an abstraction for the Opengl
* indexbuffer based on and adapted from code in the Hazel 
* Github repository (see Game.cpp citation [9])
*/
class IndexBuffer {
public:
	IndexBuffer(unsigned int length);
	~IndexBuffer();

	void bind();
	void unbind();

	unsigned int& operator[] (int index) {
		return m_data[index];
	}

	unsigned int length() { return m_length; }
	void bindData();

private:
	static unsigned int s_boundBuffer;

	unsigned int m_length;
	unsigned int* m_data;
	unsigned int m_id;
};