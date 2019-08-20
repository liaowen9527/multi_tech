#include "MongoUri.h"

MongoUri::MongoUri(const std::string& ip, int port /*= 27017*/)
	: m_ip(ip)
	, m_port(port)
{

}

std::string MongoUri::to_string() const
{
	return "";
}

