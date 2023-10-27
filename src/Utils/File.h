#pragma once

#include <string>
#include "Exception.h"

class File
{
public:
	class NotFoundException: public Exception
	{
	public:
		NotFoundException(const std::string& targetFile) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
	private:
		std::string m_TargetFile;
	};
public:
	static std::string Read(const std::string& file);
};
