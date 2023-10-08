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
		return "";
	}
}
