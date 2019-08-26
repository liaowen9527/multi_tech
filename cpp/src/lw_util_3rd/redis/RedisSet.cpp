#include "RedisSet.h"
#include "acl_cpp/lib_acl.hpp"
#include "RedisClient.h"
#include "RedisClientCluster.h"
#include "AclStringHelper.h"

RedisSet::RedisSet(void)
	: m_pClient(nullptr)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisSet::RedisSet(RedisClient* conn)
	: m_pClient(conn)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisSet::RedisSet(RedisClientCluster* cluster, size_t max_conns /*= 0*/)
	: m_pClient(nullptr)
	, m_pCluster(cluster)
	, m_max_conns(max_conns)
{

}

RedisSet::~RedisSet(void)
{

}

void RedisSet::init_redis_set(acl::redis_set* cmd)
{
	if (m_pClient)
	{
		cmd->set_client(m_pClient->m_redis_client);
	}
	else if (m_pCluster)
	{
		cmd->set_cluster(m_pCluster->m_redis_cluster, m_max_conns);
	}
}

int RedisSet::sadd(const char* key, const std::vector<std::string>& members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclmembers = AclStringHelper().to_aclstring_vector(members);

	return cmd.sadd(key, aclmembers);
}

bool RedisSet::spop(const char* key, std::string& buf)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	acl::string str;
	auto ret = cmd.spop(key, str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

int RedisSet::scard(const char* key)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	return cmd.scard(key);
}

int RedisSet::smembers(const char* key, std::vector<std::string>* members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	if (NULL == members)
	{
		return cmd.smembers(key, NULL);
	}

	std::vector<acl::string> aclmembers;
	auto ret = cmd.smembers(key, &aclmembers);

	AclStringHelper().to_string_vector(aclmembers, *members);

	return ret;
}

int RedisSet::smove(const char* src, const char* dst, const std::string& member)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	return cmd.smove(src, dst, member.c_str(), member.length());
}

int RedisSet::sdiff(const std::vector<std::string>& keys, std::vector<std::string>* members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	AclStringHelper helper;
	std::vector<acl::string> aclkeys = helper.to_aclstring_vector(keys);

	if (NULL == members)
	{
		return cmd.sdiff(aclkeys, NULL);
	}

	std::vector<acl::string> aclmembers;
	auto ret = cmd.sdiff(aclkeys, &aclmembers);
	helper.to_string_vector(aclmembers, *members);

	return ret;
}

int RedisSet::sinter(const std::vector<std::string>& keys, std::vector<std::string>* members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	AclStringHelper helper;
	std::vector<acl::string> aclkeys = helper.to_aclstring_vector(keys);

	if (NULL == members)
	{
		return cmd.sdiff(aclkeys, NULL);
	}

	std::vector<acl::string> aclmembers;
	auto ret = cmd.sinter(aclkeys, &aclmembers);
	helper.to_string_vector(aclmembers, *members);

	return ret;
}

int RedisSet::sunion(const std::vector<std::string>& keys, std::vector<std::string>* members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	AclStringHelper helper;
	std::vector<acl::string> aclkeys = helper.to_aclstring_vector(keys);

	if (NULL == members)
	{
		return cmd.sdiff(aclkeys, NULL);
	}

	std::vector<acl::string> aclmembers;
	auto ret = cmd.sunion(aclkeys, &aclmembers);
	helper.to_string_vector(aclmembers, *members);

	return ret;
}

int RedisSet::sdiffstore(const char* dst, const std::vector<std::string>& keys)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclkeys = AclStringHelper().to_aclstring_vector(keys);

	return cmd.sdiffstore(dst, aclkeys);
}

int RedisSet::sinterstore(const char* dst, const std::vector<std::string>& keys)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclkeys = AclStringHelper().to_aclstring_vector(keys);

	return cmd.sinterstore(dst, aclkeys);
}

int RedisSet::sunionstore(const char* dst, const std::vector<std::string>& keys)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclkeys = AclStringHelper().to_aclstring_vector(keys);

	return cmd.sunionstore(dst, aclkeys);
}

bool RedisSet::sismember(const char* key, const std::string& member)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	return cmd.sismember(key, member.c_str(), member.length());
}

int RedisSet::srandmember(const char* key, std::string& out)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	acl::string str;
	auto ret = cmd.srandmember(key, str);

	AclStringHelper().to_string(str, out);

	return ret;
}

int RedisSet::srandmember(const char* key, size_t n, std::vector<std::string>& out)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclouts;
	auto ret = cmd.srandmember(key, n, aclouts);

	AclStringHelper().to_string_vector(aclouts, out);

	return ret;
}

int RedisSet::srem(const char* key, const std::vector<std::string>& members)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclmembers = AclStringHelper().to_aclstring_vector(members);

	return cmd.srem(key, aclmembers);
}

int RedisSet::sscan(const char* key, int cursor, std::vector<std::string>& out, const char* pattern /*= NULL*/, const size_t* count /*= NULL*/)
{
	acl::redis_set cmd;
	init_redis_set(&cmd);

	std::vector<acl::string> aclouts;
	auto ret = cmd.sscan(key, cursor, aclouts, pattern, count);

	AclStringHelper().to_string_vector(aclouts, out);

	return ret;
}
