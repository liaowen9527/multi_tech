#include "RedisClient.h"
#include "acl_cpp/lib_acl.hpp"

RedisClient::RedisClient()
	: m_redis_client(nullptr)
{
	
}

RedisClient::RedisClient(const char* addr, int conn_timeout /*= 60*/, int rw_timeout /*= 30*/, bool retry /*= true*/)
{
	init(addr, conn_timeout, rw_timeout, retry);
}

RedisClient::~RedisClient()
{
	if (m_redis_client)
	{
		delete m_redis_client;
		m_redis_client = nullptr;
	}
}

void RedisClient::InitEnv()
{
	acl::acl_cpp_init();
}

void RedisClient::init(const char* addr, int conn_timeout /*= 60*/, int rw_timeout /*= 30*/, bool retry /*= true*/)
{
	m_redis_client = new acl::redis_client(addr, conn_timeout, rw_timeout, retry);
}

void RedisClient::set_password(const char* pass)
{
	m_redis_client->set_password(pass);
}

void RedisClient::set_timeout(int conn_timeout /*= 60*/, int rw_timeout /*= 30*/, bool retry /*= true*/)
{
	m_redis_client->set_timeout(conn_timeout, rw_timeout);
}

RedisString RedisClient::String()
{
	return RedisString(this);
}
