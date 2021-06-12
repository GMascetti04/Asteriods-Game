#pragma once
#include "xpch.h"

#include "Engine/Debug/Profiler.h"
/*
* The Shader class is an abstraction for the OpenGL shader and is modified and 
* adapted from code in the Hazel Github repository (see Game.cpp citation [9])
*/
class Shader {
public:

	enum class ShaderType {
		vertex = 0x8B31,
		fragment = 0x8B30,
		geometry = 0x8DD9
	};

public:

	Shader(Shader::ShaderType type) : m_id(0), m_type(type) {}
	~Shader() {}

	void CompileString(std::string& str);

	unsigned int getID() { return m_id; }
	Shader::ShaderType getType() { return m_type; }

private:
	unsigned int m_id;
	Shader::ShaderType m_type;
};