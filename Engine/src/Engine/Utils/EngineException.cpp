#include "engine_pch.h"
#include "EngineException.h"

namespace Engine
{
#pragma region EngineException

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
	
#pragma endregion

#pragma region EngineWException

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

#pragma endregion

#pragma region ResourceNotFoundException

	ResourceNotFoundException::ResourceNotFoundException(int line, const char* file, const std::string& path)
		:
		EngineException(line, file), m_NotFoundPath(path)
	{
	}
	const char* ResourceNotFoundException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Missing File] " << GetNotFoundPath() << std::endl
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

#pragma endregion

#pragma region InitializationError

	InitializationError::InitializationError(int line, const char* file, const std::string& errorDetails) noexcept
		:
		EngineException(line, file),
		m_Details(errorDetails)
	{
	}
	const char* InitializationError::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Details] " << GetErrorDetails() << std::endl
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* InitializationError::GetType() const noexcept
	{
		return "Initialization Error";
	}
	std::string InitializationError::GetErrorDetails() const noexcept
	{
		return m_Details;
	}

#pragma endregion

#pragma region HrException
	HrException::HrException(int line, const char* file, HRESULT hr) noexcept
		:
		EngineException(line, file), m_Hr(hr)
	{
	}

	const char* HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* HrException::GetType() const noexcept
	{
		return "Win32 HRESULT Exception";
	}

	HRESULT HrException::GetErrorCode() const noexcept
	{
		return m_Hr;
	}

	std::string HrException::GetErrorDescription() const noexcept
	{
		LPSTR msgText = nullptr;

		DWORD msgLen = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			m_Hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&msgText,
			0,
			NULL
		);

		if (msgLen == 0) return "Undefined Error Code";

		std::string errorString = msgText;
		LocalFree(msgText);
		return errorString;
	}
#pragma endregion
}
