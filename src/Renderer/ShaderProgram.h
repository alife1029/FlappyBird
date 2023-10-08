#pragma once 

#include <string>
#include <cstdint>

class Shader
{
public:
	Shader(const std::string& vsFile, const std::string& fsFile);
	~Shader();
	void Activate() const noexcept;
private:
	uint32_t m_ShaderProgram;
};
