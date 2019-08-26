#include "RedisClientCluster.h"
#include "acl_cpp/lib_acl.hpp"

RedisClientCluster::RedisClientCluster()
{

}

RedisClientCluster::~RedisClientCluster()
{

}

RedisString RedisClientCluster::String(size_t max_conns/* = 0*/)
{
	return RedisString(this, max_conns);
}
