#pragma once
#include "xpch.h"
#include "Shader.h"
#include "Engine/Debug/Profiler.h"

/*
* The ShaderProgram class is an abstraction for the OpenGL shaderprogram and is modified and
* adapted from code in the Hazel Github repository (see Game.cpp citation [9])
*/
class ShaderProgram {
public:
	ShaderProgram(Shader vert, Shader frag);
	ShaderProgram(Shader vert, Shader geom, Shader frag);
	~ShaderProgram();

	void bind();
	void unbind();

	int getUniformLocation(const char* name);

	void setUniform1i(const char* name, int val);

	void setUniform1f(const char* name, glm::vec1 vec);
	void setUniform2f(const char* name, glm::vec2 vec);
	void setUniform3f(const char* name, glm::vec3 vec);
	void setUniform4f(const char* name, glm::vec4 vec);

	void setUniformMat4f(const char* name, glm::mat4 mat);

	void setUniform1iv(const char* name, int count, int* data);


	unsigned int getId() { return m_id; }

private:
	static unsigned int s_bound;
	unsigned int m_id;
	std::unordered_map<const char*, int> m_cache;
};