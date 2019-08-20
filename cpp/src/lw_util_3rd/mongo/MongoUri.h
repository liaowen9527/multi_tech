#pragma once
#include <string>
#include "lw_util_3rd.h"

class LWUtil3rd_API MongoUri
{
public:
	MongoUri(const std::string& ip, int port = 27017);

public:
	std::string to_string() const;

public:
	std::string m_ip;
	int m_port;
	std::string m_user;
	std::string m_password;
	bool m_ssl;
	std::string m_certificate;
};
