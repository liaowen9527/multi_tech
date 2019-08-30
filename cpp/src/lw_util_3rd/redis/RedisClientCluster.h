#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "RedisString.h"
#include "RedisHash.h"
#include "RedisList.h"
#include "RedisSet.h"

namespace acl {
	class redis_client_cluster;
}

class LWUtil3rd_API RedisClientCluster
{
	friend RedisString;
	friend RedisHash;
	friend RedisList;
	friend RedisSet;

public:
	RedisClientCluster();
	~RedisClientCluster();

public:
	void set(const char* addr, size_t count, int conn_timeout = 30, int rw_timeout = 30);
	void set_password(const char* addr, const char* pass);

public:
	RedisString String(size_t max_conns = 0);
	RedisList List(size_t max_conns = 0);
	RedisSet Set(size_t max_conns = 0);
	RedisHash Hash(size_t max_conns = 0);

protected:
	acl::redis_client_cluster* m_redis_cluster;

};