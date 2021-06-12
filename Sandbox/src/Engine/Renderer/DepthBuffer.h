#pragma once
#include "GL/glew.h"

class FrameBufferAttachment
{
public:
	virtual int getType() = 0;
	virtual unsigned int getID() = 0;
};
class DepthBuffer : public FrameBufferAttachment
{
public:
	DepthBuffer(int width, int height)
	{
		glGenRenderbuffers(1, &m_id);
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	}
	virtual int getType() override { return GL_DEPTH_ATTACHMENT; }
	virtual unsigned int getID() override { return m_id; }

	void bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}
private:
	unsigned int m_id;
};

