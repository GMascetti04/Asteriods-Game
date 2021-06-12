#pragma once
#include "GL/glew.h"
#include "Engine/Debug/Profiler.h"


/*
* The VertexBuffer class is an abstraction for the OpenGL VertexBuffer and is modified and
* adapted from code in the Hazel Github repository (see Game.cpp citation [9])
*/
template<typename VertexClass>
class VertexBuffer {
public:
	
	VertexBuffer(unsigned int length) : m_length(length) {
		glGenBuffers(1, &m_id);
		m_data = new VertexClass[m_length];
		bind();
		glBufferData(GL_ARRAY_BUFFER, m_length * sizeof(VertexClass)
			, nullptr, GL_DYNAMIC_DRAW);
	}
	~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
		unbind();
		delete[] m_data;
	}

	void bind() {
		
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//glBufferData(GL_ARRAY_BUFFER, m_length * sizeof(VertexClass)
		//	, m_data, GL_DYNAMIC_DRAW);
			
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	VertexClass& operator[] (int index)
	{
		return m_data[index];
	}

	unsigned int length() { return m_length; }

	void bindData() {
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_length * sizeof(VertexClass), m_data);
	}

private:

	unsigned int m_length;
	VertexClass* m_data;
	unsigned int m_id;
};