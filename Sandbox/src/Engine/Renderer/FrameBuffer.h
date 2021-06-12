#pragma once
#include "GL/glew.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/DepthBuffer.h"
class FrameBuffer
{
public:
	FrameBuffer(int width, int height) {
		glGenFramebuffers(1, &m_id);
		m_texture = Texture::createRenderTexture(width, height, GL_RGB);
		bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->getId(), 0);
		SetBufferList();

		//check to see if it worked
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			glDeleteFramebuffers(1, &m_id);
			delete m_texture;

		}
	}

	void AttachBuffer(FrameBufferAttachment* attachment)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment->getType(), GL_RENDERBUFFER, attachment->getID());
	}

	void SetBufferList()
	{
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	void AttachTexture() //might remove
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->getId(), 0);
	}

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	~FrameBuffer()
	{

	}
private:
	unsigned int m_id;
	Texture* m_texture;
};

