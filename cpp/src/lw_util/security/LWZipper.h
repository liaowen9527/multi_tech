#pragma once
#include <string>

class LWZipper
{
public:
	static bool Zip(const std::string& data, std::string& zipData);
	static bool Unzip(const std::string& zipData, std::string& data);
};