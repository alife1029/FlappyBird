#pragma once 

#include <string>
#include <cstdint>

#include "../Utils/Exception.h"

class Shader
{
public:
	class CompilationError : public Exception
	{
	public:
		CompilationError(int line, const char* file, const std::string& shaderFile, unsigned int shaderType, const std::string& gpuLog)
			:
			Exception(line, file),
			m_ShaderFile(shaderFile),
			m_ShaderType(shaderType),
			m_GpuLog(gpuLog) { }
		virtual const char* GetType() const noexcept { return "Shader Compilation Error"; }
		const char* what() const noexcept;
		const std::string& GetShaderFile() const noexcept { return m_ShaderFile; }
		unsigned int GetShaderType() const noexcept { return m_ShaderType; }
		const std::string& GPULog() const noexcept { return m_GpuLog; }
	private:
		std::string m_ShaderFile;
		std::string m_GpuLog;
		unsigned int m_ShaderType;
	};

	class LinkageError : public Exception
	{
	public:
		LinkageError(int line, const char* file, const std::string& vsFile, const std::string& fsFile, const std::string& gpuLog)
			:
			Exception(line, file),
			m_VsFile(vsFile),
			m_FsFile(fsFile),
			m_GpuLog(gpuLog) { }
		virtual const char* GetType() const noexcept { return "Shader Program Linkage Error"; }
		const char* what() const noexcept;
		const std::string& GetGPULog() const noexcept { return m_GpuLog; }
		const std::string& GetVSFile() const noexcept { return m_VsFile; }
		const std::string& GetFSFile() const noexcept { return m_FsFile; }
	private:
		std::string	m_GpuLog,
					m_VsFile,
					m_FsFile;
	};
public:
	Shader(const std::string& vsFile, const std::string& fsFile);
	~Shader();
	void Activate() const noexcept;
private:
	uint32_t m_ShaderProgram;
};
