#include "RedisClientCluster.h"
#include "acl_cpp/lib_acl.hpp"

RedisClientCluster::RedisClientCluster()
{
	m_redis_cluster = new acl::redis_client_cluster;
}

RedisClientCluster::~RedisClientCluster()
{
	if (m_redis_cluster)
	{
		delete m_redis_cluster;
		m_redis_cluster = nullptr;
	}
}

void RedisClientCluster::set(const char* addr, size_t count, int conn_timeout /*= 30*/, int rw_timeout /*= 30*/)
{
	m_redis_cluster->set(addr, count, conn_timeout, rw_timeout);
}

void RedisClientCluster::set_password(const char* addr, const char* pass)
{
	m_redis_cluster->set_password(addr, pass);
}

RedisString RedisClientCluster::String(size_t max_conns/* = 0*/)
{
	return RedisString(this, max_conns);
}

RedisList RedisClientCluster::List(size_t max_conns /*= 0*/)
{
	return RedisList(this, max_conns);
}

RedisSet RedisClientCluster::Set(size_t max_conns /*= 0*/)
{
	return RedisSet(this, max_conns);
}

RedisHash RedisClientCluster::Hash(size_t max_conns /*= 0*/)
{
	return RedisHash(this, max_conns);
}
