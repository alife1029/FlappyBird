#include "ShaderProgram.h"

#include <glad/glad.h>

#include <iostream>
#include <sstream>

#include "../Utils/File.h"

void AttachShader(uint32_t program, const std::string& sourceFile, uint32_t shaderType)
{
	uint32_t shader = glCreateShader(shaderType);
	std::string shaderStringSource = File::Read(sourceFile);
	const char* shaderSource = shaderStringSource.c_str();
	
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// Compilation error
	GLint isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		constexpr GLsizei log_size = 1024;
		char log[log_size];
		glGetShaderInfoLog(shader, log_size, nullptr, log);

		throw Shader::CompilationError(__LINE__, __FILE__, sourceFile, shaderType, log);
	}

	glAttachShader(program, shader);
	glDeleteShader(shader);
}

Shader::Shader(const std::string& vsFile, const std::string& fsFile)
{
	m_ShaderProgram = glCreateProgram();
	AttachShader(m_ShaderProgram, vsFile, GL_VERTEX_SHADER);
	AttachShader(m_ShaderProgram, fsFile, GL_FRAGMENT_SHADER);

	// Link shader program
	glLinkProgram(m_ShaderProgram);

	// Linkage error
	GLint isLinked;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		constexpr GLsizei log_size = 1024;
		char log[log_size];
		glGetProgramInfoLog(m_ShaderProgram, log_size, nullptr, log);

		throw LinkageError(__LINE__, __FILE__, vsFile, fsFile, log);
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Activate() const noexcept
{
	glUseProgram(m_ShaderProgram);
}

const char* Shader::CompilationError::what() const noexcept
{
	std::string _shaderType = "Unknown Shader Type";
	switch (GetShaderType())
	{
	case GL_VERTEX_SHADER:
		_shaderType = "Vertex shader";
		break;
	case GL_FRAGMENT_SHADER:
		_shaderType = "Fragment Shader";
		break;
	}

	std::ostringstream oss;
	oss << GetType() << std::endl 
		<< "[Shader File] " << GetShaderFile() << std::endl
		<< "[Shader Type] " << _shaderType << std::endl
		<< "\n[GPU Log] " << std::endl
		<< GPULog();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* Shader::LinkageError::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "[Vertex Shader File] " << GetVSFile() << std::endl
		<< "[Fragment Shader File] " << GetFSFile() << std::endl
		<< "\n[GPU Log] " << std::endl
		<< GetGPULog() << std::endl;

	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}
