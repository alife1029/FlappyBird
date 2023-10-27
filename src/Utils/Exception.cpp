#include "Exception.h"

#include <sstream>

Exception::Exception(int line, const char* file) noexcept
	:
	m_Line(line),
	m_File(file)
{ }

const char* Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* Exception::GetType() const noexcept
{
	return "Game Engine Exception";
}

int Exception::GetLine() const noexcept
{
	return m_Line;
}

const std::string& Exception::GetFile() const noexcept
{
	return m_File;
}

std::string Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;

	oss << "[File] " << m_File << std::endl
		<< "[Line] " << m_Line;

	return oss.str();
}

InitializationException::InitializationException(int line, const char* file, const std::string& failedInit) noexcept
	:
	Exception(line, file),
	m_ObjectCouldntInitialized(failedInit)
{}

const char* InitializationException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< GetOriginString() << std::endl
		<< "Failed to initialize " << GetObjectCouldntInitialized() << std::endl;

	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const char* InitializationException::GetType() const noexcept
{
	return "Initialization Error";
}
