#include "engine_pch.h"
#include "StringUtils.h"

namespace Engine
{
	std::wstring ToWideString(const std::string& str)
	{
		wchar_t ret[512];
		mbstowcs_s(nullptr, ret, str.c_str(), _TRUNCATE);
		return ret;
	}

	std::string ToNarrowString(const std::wstring& wstr)
	{
		char ret[512];
		wcstombs_s(nullptr, ret, wstr.c_str(), _TRUNCATE);
		return ret;
	}
}
