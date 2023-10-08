#include "ShaderProgram.h"

#include <glad/glad.h>

#include <iostream>

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

		// TODO: Throw exception
		std::cout	<< "FAILED TO COMPILE SHADER!\t(SHADER_FILE: " << sourceFile << ")" << std::endl
					<< "GPU_LOG:\n" << std::endl
					<< log << std::endl;

		goto deleteshader;
	}

	glAttachShader(program, shader);
	
	// Cleanup
deleteshader:
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

		// TODO: Throw exception
		std::cout	<< "FAILED TO LINK SHADER PROGRAM!" << std::endl
					<< "GPU_LOG:\n" << std::endl
					<< log << std::endl;
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
