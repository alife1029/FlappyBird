#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept;
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

class InitializationException : public Exception
{
public:
	InitializationException(int line, const char* file, const std::string& failedInit) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	std::string GetObjectCouldntInitialized() const noexcept { return m_ObjectCouldntInitialized; }
private:
	std::string m_ObjectCouldntInitialized;
};

#define THROW_INITIALIZATION_EXCEPTION(_failed_init_) throw InitializationException(__LINE__, __FILE__, _failed_init_)
