#pragma once

#include <string>

namespace Engine
{
	std::wstring ToWideString(const std::string& str);
	std::string ToNarrowString(const std::wstring& wstr);
}
