#include "MongoDBUri.h"
#include <mongoc/mongoc.h>
#include "log/LWLog.h"
const std::string MongoDBUri::k_default_uri = "mongodb://localhost:27017";

#ifdef GetUserName
#undef GetUserName
#endif

mongoc_uri_t * lw_new_mongoc_uri(const char *uri_string)
{
	bson_error_t error;
	mongoc_uri_t *c_uri = mongoc_uri_new_with_error(uri_string, &error);
	if (!c_uri)
	{
		LW_LOG(InstMongo, LOG_ERROR, "failed to parse URI: %s, error message: %s", uri_string, error.message);
	};
	return c_uri;
}

MongoDBUri::MongoDBUri()
{
	m_uri = lw_new_mongoc_uri(k_default_uri.c_str());
}

MongoDBUri::MongoDBUri(MongoDBUri&& r)
{

}

MongoDBUri::~MongoDBUri()
{
	if (m_uri)
	{
		mongoc_uri_destroy(m_uri);
	}
}

bool MongoDBUri::GetUri(const std::string& uri_string, MongoDBUri& uri)
{
	mongoc_uri_t *c_uri = lw_new_mongoc_uri(k_default_uri.c_str());
	if (!c_uri) 
	{
		return false;
	};
	
	uri.m_uri = c_uri;
	return true;
}

MongoDBUri& MongoDBUri::operator=(const MongoDBUri& r)
{
	m_uri = mongoc_uri_copy(r.m_uri);
	return (*this);
}

const mongoc_uri_t* MongoDBUri::GetMongocUri() const
{
	return m_uri;
}

std::vector<MongoDBUri::Host> MongoDBUri::GetHosts()
{
	std::vector<MongoDBUri::Host> ret;
	const mongoc_host_list_t* hosts = mongoc_uri_get_hosts(m_uri);
	while (hosts)
	{
		Host h;
		h.host = hosts->host;
		h.port = hosts->port;

		ret.push_back(h);
		hosts = hosts->next;
	}

	return std::move(ret);
}

const char* MongoDBUri::GetUserName() const
{
	return mongoc_uri_get_username(m_uri);
}

void MongoDBUri::SetUserName(const std::string& str)
{
	mongoc_uri_set_username(m_uri, str.c_str());
}

const char* MongoDBUri::GetPassword() const
{
	return mongoc_uri_get_password(m_uri);
}

void MongoDBUri::SetPassword(const std::string& str)
{
	mongoc_uri_set_password(m_uri, str.c_str());
}

bool MongoDBUri::Ssl() const
{
	return mongoc_uri_get_ssl(m_uri);
}

void MongoDBUri::EnableSsh(bool v)
{
	
}

