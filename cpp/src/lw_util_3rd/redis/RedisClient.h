#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "RedisString.h"
#include "RedisHash.h"
#include "RedisList.h"
#include "RedisSet.h"

namespace acl {
	class redis_client;
}

class LWUtil3rd_API RedisClient
{
	friend RedisString;
	friend RedisHash;
	friend RedisList;
	friend RedisSet;

public:
	RedisClient();
	RedisClient(const char* addr, int conn_timeout = 60, int rw_timeout = 30, bool retry = true);
	~RedisClient();

	static void InitEnv();

public:
	void init(const char* addr, int conn_timeout = 60, int rw_timeout = 30, bool retry = true);
	
	void set_password(const char* pass);
	void set_timeout(int conn_timeout = 60, int rw_timeout = 30, bool retry = true);

public:
	RedisString String();

protected:
	acl::redis_client* m_redis_client;
};