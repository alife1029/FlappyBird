#pragma once

#include <exception>
#include <string>

namespace Engine
{
	class EngineException : public std::exception
	{
	public:
		EngineException(int line, const char* file) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	
	private:
		int m_Line;
		std::string m_File;

	protected:
		mutable std::string m_WhatBuffer;
	};

	class EngineWException : public std::exception
	{
	public:
		EngineWException(int line, const wchar_t* file) noexcept;
		const char* what() const noexcept override;
		virtual const wchar_t* what_unicode() const noexcept;
		virtual const wchar_t* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::wstring& GetFile() const noexcept;
		std::wstring GetOriginString() const noexcept;

	private:
		int m_Line;
		std::wstring m_File;

	protected:
		mutable std::wstring m_WhatBuffer;
	};

	class ResourceNotFoundException : public EngineException
	{
	public:
		ResourceNotFoundException(int line, const char* file, const std::string& path);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;

		std::string GetNotFoundPath() const noexcept;

	private:
		std::string m_NotFoundPath;
	};
}
