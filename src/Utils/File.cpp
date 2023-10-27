#include "File.h"

#include <fstream>
#include <sstream>

std::string File::Read(const std::string& file)
{
	std::ifstream ifs(file);
	if (ifs.is_open())
	{
		std::ostringstream oss;
		oss << ifs.rdbuf();
		ifs.close();
		return oss.str();
	}
	else
	{
		// TODO: Throw file not found exception
		throw NotFoundException(file);
	}
}

File::NotFoundException::NotFoundException(const std::string& targetFile) noexcept
	:
	Exception(0, "---"),
	m_TargetFile(targetFile) { }

const char* File::NotFoundException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "[File] " << m_TargetFile << std::endl;

	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const char* File::NotFoundException::GetType() const noexcept
{
	return "File Not Found";
}


