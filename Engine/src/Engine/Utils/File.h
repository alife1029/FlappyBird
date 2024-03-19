#pragma once

#include <string>

namespace Engine
{
	class File
	{
	public:
		static std::string Read(const std::string& path);
	};
}
