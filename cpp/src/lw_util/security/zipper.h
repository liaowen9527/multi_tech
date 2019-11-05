#pragma once
#include <string>

class Zipper
{
public:
	static bool Zip(const std::string& data, std::string& zipData);
	static bool Unzip(const std::string& zipData, std::string& data);
};