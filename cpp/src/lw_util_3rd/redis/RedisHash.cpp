#include "RedisHash.h"
#include "acl_cpp/lib_acl.hpp"
#include "RedisClient.h"
#include "RedisClientCluster.h"
#include "AclStringHelper.h"

RedisHash::RedisHash(void)
	: m_pClient(nullptr)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisHash::RedisHash(RedisClient* conn)
	: m_pClient(conn)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisHash::RedisHash(RedisClientCluster* cluster, size_t max_conns /*= 0*/)
	: m_pClient(nullptr)
	, m_pCluster(cluster)
	, m_max_conns(max_conns)
{

}

RedisHash::~RedisHash(void)
{

}

void RedisHash::init_redis_hash(acl::redis_hash* cmd)
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

bool RedisHash::hmset(const char* key, const std::map<std::string, std::string>& attrs)
{
	std::vector<const char*> names_result;
	std::vector<const char*> values_result;
	std::vector<size_t> names_len;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(attrs, names_result, names_len, values_result, values_len);

	return hmset(key, &names_result[0], &names_len[0], &values_result[0], &values_len[0], attrs.size());
}

bool RedisHash::hmset(const char* key, const std::vector<std::string>& names, const std::vector<std::string>& values)
{
	std::vector<const char*> names_result;
	std::vector<const char*> values_result;
	std::vector<size_t> names_len;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(names, values, names_result, names_len, values_result, values_len);

	return hmset(key, &names_result[0], &names_len[0], &values_result[0], &values_len[0], names.size());
}

bool RedisHash::hmset(const char* key, const char* names[], const size_t names_len[], const char* values[], const size_t values_len[], size_t argc)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hmset(key, names, names_len, values, values_len, argc);
}

bool RedisHash::hmget(const char* key, const std::vector<std::string>& names)
{
	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	AclStringHelper helper;
	helper.to_vector_chars(names, names_result, names_len);

	return hmget(key, &names_result[0], &names_len[0], names.size());
}

bool RedisHash::hmget(const char* key, const std::vector<std::string>& names, std::vector<std::string>& result /*= NULL*/)
{
	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	AclStringHelper helper;
	helper.to_vector_chars(names, names_result, names_len);

	return hmget(key, &names_result[0], &names_len[0], names.size(), &result);
}

bool RedisHash::hmget(const char* key, const char* names[], const size_t lens[], size_t argc, std::vector<std::string>* result /*= NULL*/)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	if (NULL == result)
	{
		return cmd.hmget(key, names, argc, NULL);
	}

	std::vector<acl::string> strs;
	auto ret = cmd.hmget(key, names, lens, argc, &strs);

	AclStringHelper helper;
	helper.to_string_vector(strs, *result);

	return ret;
}

int RedisHash::hset(const char* key, const std::string& name, const std::string& value)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hset(key, name.c_str(), name.length(), value.c_str(), value.length());
}

int RedisHash::hsetnx(const char* key, const std::string& name, const std::string& value)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hsetnx(key, name.c_str(), name.length(), value.c_str(), value.length());
}

bool RedisHash::hget(const char* key, const std::string& name, std::string& result)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	acl::string str;
	auto ret = cmd.hget(key, name.c_str(), name.length(), str);

	AclStringHelper helper;
	helper.to_string(str, result);
	
	return ret;
}

bool RedisHash::hgetall(const char* key, std::map<std::string, std::string>& result)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	std::map<acl::string, acl::string> mapAclstr;
	auto ret = cmd.hgetall(key, mapAclstr);

	AclStringHelper helper;
	helper.to_string_map(mapAclstr, result);

	return ret;
}

bool RedisHash::hgetall(const char* key, std::vector<std::string>& names, std::vector<std::string>& values)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	AclStringHelper helper;

	std::vector<acl::string> aclnames = helper.to_aclstring_vector(names);
	std::vector<acl::string> aclvalues;
	auto ret = cmd.hgetall(key, aclnames, aclvalues);

	helper.to_string_vector(aclvalues, values);

	return ret;
}

int RedisHash::hdel(const char* key, const char* first_name)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hdel(key, first_name);
}

int RedisHash::hdel(const char* key, const std::vector<std::string>& names)
{
	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	AclStringHelper helper;
	helper.to_vector_chars(names, names_result, names_len);

	return hdel(key, names_result);
}

int RedisHash::hdel(const char* key, const std::vector<const char*>& names)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hdel(key, names);
}

int RedisHash::hdel_fields(const char* key, const std::vector<std::string>& names)
{
	std::vector<const char*> names_result;
	std::vector<size_t> names_len;

	AclStringHelper helper;
	helper.to_vector_chars(names, names_result, names_len);

	return hdel_fields(key, names_result);
}

int RedisHash::hdel_fields(const char* key, const std::vector<const char*>& names)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hdel_fields(key, names);
}

bool RedisHash::hincrby(const char* key, const char* name, long long int inc, long long int* result /*= NULL*/)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hdel_fields(key, name, inc, result);
}

bool RedisHash::hincrbyfloat(const char* key, const char* name, double inc, double* result /*= NULL*/)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hincrbyfloat(key, name, inc, result);
}

bool RedisHash::hkeys(const char* key, std::vector<std::string>& names)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	std::vector<acl::string> aclnames;
	auto ret = cmd.hkeys(key, aclnames);

	AclStringHelper helper;
	helper.to_string_vector(aclnames, names);

	return ret;
}

bool RedisHash::hexists(const char* key, const char* name)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hexists(key, name);
}

bool RedisHash::hexists(const char* key, const char* name, size_t name_len)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hexists(key, name, name_len);
}

int RedisHash::hlen(const char* key)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hlen(key);
}

int RedisHash::hstrlen(const char* key, const char* name, size_t name_len)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hstrlen(key, name, name_len);
}

int RedisHash::hstrlen(const char* key, const char *name)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	return cmd.hstrlen(key, name);
}

int RedisHash::hscan(const char* key, int cursor, std::map<std::string, std::string>& out, const char* pattern /*= NULL*/, const size_t* count /*= NULL*/)
{
	acl::redis_hash cmd;
	init_redis_hash(&cmd);

	std::map<acl::string, acl::string> aclout;
	auto ret = cmd.hscan(key, cursor, aclout, pattern, count);

	AclStringHelper helper;
	helper.to_string_map(aclout, out);

	return ret;
}
