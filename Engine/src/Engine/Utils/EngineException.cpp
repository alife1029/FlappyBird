#include "engine_pch.h"
#include "EngineException.h"

namespace Engine
{
	EngineException::EngineException(int line, const char* file) noexcept
		: m_Line(line), m_File(file)
	{
	}
	const char* EngineException::what() const noexcept
	{
		std::ostringstream oss;

		oss << GetType() << std::endl
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* EngineException::GetType() const noexcept
	{
		return "Engine Exception";
	}
	int EngineException::GetLine() const noexcept
	{
		return m_Line;
	}
	const std::string& EngineException::GetFile() const noexcept
	{
		return m_File;
	}
	std::string EngineException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << std::endl
			<< "[Line] " << m_Line;
		return oss.str();
	}


	EngineWException::EngineWException(int line, const wchar_t* file) noexcept
		:
		m_Line(line), m_File(file)
	{
	}
	const char* EngineWException::what() const noexcept
	{
		const wchar_t* unicode_what = what_unicode();
		size_t outputSize = m_WhatBuffer.size() + 1;
		char* output = new char[outputSize];
		wcstombs_s(nullptr, output, outputSize, unicode_what, _TRUNCATE);
		return output;
	}
	const wchar_t* EngineWException::what_unicode() const noexcept
	{
		std::wostringstream wos;
		wos << GetType() << std::endl
			<< GetOriginString();
		m_WhatBuffer = wos.str();
		return m_WhatBuffer.c_str();
	}
	const wchar_t* EngineWException::GetType() const noexcept
	{
		return L"Engine Exception";
	}
	int EngineWException::GetLine() const noexcept
	{
		return m_Line;
	}
	const std::wstring& EngineWException::GetFile() const noexcept
	{
		return m_File;
	}
	std::wstring EngineWException::GetOriginString() const noexcept
	{
		std::wostringstream wos;
		wos << L"[File] " << m_File << std::endl
			<< L"[Line] " << m_Line;
		return wos.str();
	}

	ResourceNotFoundException::ResourceNotFoundException(int line, const char* file, const std::string& path)
		:
		EngineException(line, file), m_NotFoundPath(path)
	{
	}
	const char* ResourceNotFoundException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Missing File ]" << GetNotFoundPath() << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* ResourceNotFoundException::GetType() const noexcept
	{
		return "Resource Not Found";
	}
	std::string ResourceNotFoundException::GetNotFoundPath() const noexcept
	{
		return m_NotFoundPath;
	}
}
