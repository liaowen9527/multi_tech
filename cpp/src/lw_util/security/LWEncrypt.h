#pragma once
#include <string>

class LWEncrypt
{
public:
	LWEncrypt();
	~LWEncrypt();

public:
	bool Encrypt(const std::string& data, std::string& enData);
	bool Decrypt(const std::string& enData, std::string& data);
};
