#include "engine_pch.h"
#include "File.h"

#include "EngineException.h"

namespace Engine
{
	std::string File::Read(const std::string& path)
	{
		std::ifstream ifs(path);
		std::stringstream ret;

		if (ifs.is_open())
		{
			ret << ifs.rdbuf();
		}
		else
		{
			throw ResourceNotFoundException(__LINE__, __FILE__, path);
		}

		return ret.str();
	}
}
