#pragma once

#include <string>

class DebugLog
{
public:
	static void Log(const std::string& message);

	static std::wstring ConvertString(const std::string& str);

	static std::string ConvertString(const std::wstring& str);
};

