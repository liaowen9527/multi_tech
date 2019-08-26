#include "RedisList.h"
#include "acl_cpp/lib_acl.hpp"
#include "RedisClient.h"
#include "RedisClientCluster.h"
#include "AclStringHelper.h"

RedisList::RedisList(void)
	: m_pClient(nullptr)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisList::RedisList(RedisClient* conn)
	: m_pClient(conn)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisList::RedisList(RedisClientCluster* cluster, size_t max_conns /*= 0*/)
	: m_pClient(nullptr)
	, m_pCluster(cluster)
	, m_max_conns(max_conns)
{

}

RedisList::~RedisList(void)
{

}

void RedisList::init_redis_list(acl::redis_list* cmd)
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

bool RedisList::blpop(const std::vector<std::string>& keys, size_t timeout, std::pair<std::string, std::string>& result)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	AclStringHelper helper;
	std::vector<acl::string> aclkeys = helper.to_aclstring_vector(keys);

	std::pair<acl::string, acl::string> acl_pair;
	auto ret = cmd.blpop(aclkeys, timeout, acl_pair);

	helper.to_string(acl_pair.first, result.first);
	helper.to_string(acl_pair.second, result.second);

	return ret;
}

bool RedisList::brpop(const std::vector<std::string>& keys, size_t timeout, std::pair<std::string, std::string>& result)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	AclStringHelper helper;
	std::vector<acl::string> aclkeys = helper.to_aclstring_vector(keys);

	std::pair<acl::string, acl::string> acl_pair;
	auto ret = cmd.brpop(aclkeys, timeout, acl_pair);

	helper.to_string(acl_pair.first, result.first);
	helper.to_string(acl_pair.second, result.second);

	return ret;
}

bool RedisList::brpoplpush(const char* src, const char* dst, size_t timeout, std::string* buf /*= NULL*/)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	if (NULL == buf)
	{
		return cmd.brpoplpush(src, dst, timeout, NULL);
	}

	acl::string str;
	auto ret = cmd.brpoplpush(src, dst, timeout, &str);

	AclStringHelper().to_string(str, *buf);

	return ret;
}

bool RedisList::lindex(const char* key, size_t idx, std::string& buf)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	acl::string str;
	auto ret = cmd.lindex(key, idx, str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

int RedisList::linsert_before(const char* key, const std::string& pivot, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.linsert_before(key, pivot.c_str(), pivot.length(), value.c_str(), value.length());
}

int RedisList::linsert_after(const char* key, const std::string& pivot, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.linsert_after(key, pivot.c_str(), pivot.length(), value.c_str(), value.length());
}

int RedisList::llen(const char* key)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.llen(key);
}

int RedisList::lpop(const char* key, std::string& buf)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	acl::string str;
	auto ret = cmd.lpop(key, str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

int RedisList::lpush(const char* key, const std::vector<std::string>& values)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_vector_chars(values, values_result, values_len);

	return cmd.lpush(key, &values_result[0], &values_len[0], values.size());
}

int RedisList::lpushx(const char* key, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.lpushx(key, value.c_str(), value.length());
}

bool RedisList::lrange(const char* key, int start, int end, std::vector<std::string>* result)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	if (NULL)
	{
		return cmd.lrange(key, start, end, NULL);
	}

	std::vector<acl::string> aclresult;
	auto ret = cmd.lrange(key, start, end, &aclresult);

	AclStringHelper helper;
	helper.to_string_vector(aclresult, *result);

	return ret;
}

int RedisList::lrem(const char* key, int count, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.lrem(key, count, value.c_str(), value.length());
}

bool RedisList::lset(const char* key, int idx, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.lset(key, idx, value.c_str(), value.length());
}

bool RedisList::ltrim(const char* key, int start, int end)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.ltrim(key, start, end);
}

int RedisList::rpop(const char* key, std::string& buf)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	acl::string str;
	auto ret = cmd.rpop(key, str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

bool RedisList::rpoplpush(const char* src, const char* dst, std::string* buf /*= NULL*/)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	if (NULL == buf)
	{
		return cmd.rpoplpush(src, dst, NULL);
	}

	acl::string str;
	auto ret = cmd.rpoplpush(src, dst, &str);

	AclStringHelper().to_string(str, *buf);

	return ret;
}

int RedisList::rpush(const char* key, const std::vector<std::string>& values)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_vector_chars(values, values_result, values_len);

	return cmd.rpush(key, &values_result[0], &values_len[0], values.size());
}

int RedisList::rpushx(const char* key, const std::string& value)
{
	acl::redis_list cmd;
	init_redis_list(&cmd);

	return cmd.rpushx(key, value.c_str(), value.length());
}
