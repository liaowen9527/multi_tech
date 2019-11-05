#pragma once
#include <string>

class Encrypter
{
public:
	Encrypter();
	~Encrypter();

public:
	bool Encrypt(const std::string& data, std::string& enData);
	bool Decrypt(const std::string& enData, std::string& data);
};
