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


public:
	RedisString String(size_t max_conns = 0);

protected:
	acl::redis_client_cluster* m_redis_cluster;

};