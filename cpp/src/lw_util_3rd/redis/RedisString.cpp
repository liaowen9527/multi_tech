#include "RedisString.h"
#include "acl_cpp/lib_acl.hpp"
#include "RedisClient.h"
#include "RedisClientCluster.h"
#include "AclStringHelper.h"

RedisString::RedisString(void)
	: m_pClient(nullptr)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisString::RedisString(RedisClient* conn)
	: m_pClient(conn)
	, m_pCluster(nullptr)
	, m_max_conns(0)
{

}

RedisString::RedisString(RedisClientCluster* cluster, size_t max_conns /*= 0*/)
	: m_pClient(nullptr)
	, m_pCluster(cluster)
	, m_max_conns(max_conns)
{

}

RedisString::~RedisString(void)
{

}

void RedisString::init_redis_string(acl::redis_string* cmd)
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

bool RedisString::set(const std::string& key, const std::string& value)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.set(key.c_str(), key.length(), value.c_str(), value.length());
}

bool RedisString::setex(const std::string& key, const std::string& value, int timeout)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.setex(key.c_str(), key.length(), value.c_str(), value.length(), timeout);
}

bool RedisString::psetex(const std::string& key, const std::string& value, int timeout)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.psetex(key.c_str(), key.length(), value.c_str(), value.length(), timeout);
}

int RedisString::setnx(const std::string& key, const std::string& value)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.setnx(key.c_str(), key.length(), value.c_str(), value.length());
}

int RedisString::append(const std::string& key, const std::string& value)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.append(key.c_str(), value.c_str(), value.length());
}

bool RedisString::get(const std::string& key, std::string& buf)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	acl::string str;
	bool ret = cmd_string.get(key.c_str(), key.length(), str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

bool RedisString::getset(const std::string& key, const std::string& value, std::string& buf)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	acl::string str;
	auto ret = cmd_string.getset(key.c_str(), key.length(), value.c_str(), value.length(), str);

	AclStringHelper().to_string(str, buf);

	return ret;
}

int RedisString::get_strlen(const std::string& key)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.get_strlen(key.c_str(), key.length());
}

int RedisString::setrange(const std::string& key, const std::string& value, unsigned offset)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.setrange(key.c_str(), key.length(), offset, value.c_str(), value.length());
}

bool RedisString::getrange(const std::string& key, std::string& value, int start, int end)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	acl::string str;
	auto ret = cmd_string.getrange(key.c_str(), key.length(), start, end, str);

	AclStringHelper().to_string(str, value);

	return ret;
}

bool RedisString::setbit_(const std::string& key, unsigned offset, bool bit)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.setbit_(key.c_str(), key.length(), offset, bit);
}

bool RedisString::getbit(const std::string& key, unsigned offset, int& bit)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.getbit(key.c_str(), key.length(), offset, bit);
}

int RedisString::bitcount(const std::string& key)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitcount(key.c_str(), key.length());
}

int RedisString::bitcount(const std::string& key, int start, int end)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitcount(key.c_str(), key.length(), start, end);
}

int RedisString::bitop_and(const char* destkey, const std::vector<std::string>& keys)
{
	return bitop_and(destkey, AclStringHelper().to_vector_chars(keys));
}

int RedisString::bitop_and(const char* destkey, const std::vector<const char*>& keys)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_and(destkey, keys);
}

int RedisString::bitop_and(const char* destkey, const char* keys[], size_t size)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_and(destkey, keys, size);
}

int RedisString::bitop_or(const char* destkey, const std::vector<std::string>& keys)
{
	return bitop_or(destkey, AclStringHelper().to_vector_chars(keys));
}

int RedisString::bitop_or(const char* destkey, const std::vector<const char*>& keys)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_or(destkey, keys);
}

int RedisString::bitop_or(const char* destkey, const char* keys[], size_t size)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_or(destkey, keys, size);
}

int RedisString::bitop_xor(const char* destkey, const std::vector<std::string>& keys)
{
	return bitop_xor(destkey, AclStringHelper().to_vector_chars(keys));
}

int RedisString::bitop_xor(const char* destkey, const std::vector<const char*>& keys)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_xor(destkey, keys);
}

int RedisString::bitop_xor(const char* destkey, const char* keys[], size_t size)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.bitop_xor(destkey, keys, size);
}

bool RedisString::mset(const std::map<std::string, std::string>& objs)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;
	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(objs, keys_result, keys_len, values_result, values_len);

	return mset(&keys_result[0], &keys_len[0], &values_result[0], &values_len[0], objs.size());
}

bool RedisString::mset(const std::vector<std::string>& keys, const std::vector<std::string>& values)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;
	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(keys, values, keys_result, keys_len, values_result, values_len);

	return mset(&keys_result[0], &keys_len[0], &values_result[0], &values_len[0], keys.size());
}

bool RedisString::mset(const char* keys[], const size_t keys_len[], const char* values[], const size_t values_len[], size_t argc)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.mset(keys, keys_len, values, values_len, argc);
}

int RedisString::msetnx(const std::map<std::string, std::string>& objs)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;
	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(objs, keys_result, keys_len, values_result, values_len);

	return msetnx(&keys_result[0], &keys_len[0], &values_result[0], &values_len[0], objs.size());
}

int RedisString::msetnx(const std::vector<std::string>& keys, const std::vector<std::string>& values)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;
	std::vector<const char*> values_result;
	std::vector<size_t> values_len;

	AclStringHelper helper;
	helper.to_keys_values(keys, values, keys_result, keys_len, values_result, values_len);

	return msetnx(&keys_result[0], &keys_len[0], &values_result[0], &values_len[0], keys.size());
}

int RedisString::msetnx(const char* keys[], const size_t keys_len[], const char* values[], const size_t values_len[], size_t argc)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.msetnx(keys, keys_len, values, values_len, argc);
}

bool RedisString::mget(const std::vector<std::string>& keys)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;

	AclStringHelper().to_vector_chars(keys, keys_result, keys_len);

	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.mget(&keys_result[0], &keys_len[0], keys.size(), nullptr);
}

bool RedisString::mget(const std::vector<std::string>& keys, std::vector<std::string>& out)
{
	std::vector<const char*> keys_result;
	std::vector<size_t> keys_len;

	AclStringHelper().to_vector_chars(keys, keys_result, keys_len);

	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);
	
	std::vector<acl::string> strs;
	auto ret = cmd_string.mget(&keys_result[0], &keys_len[0], keys.size(), &strs);

	AclStringHelper().to_string_vector(strs, out);

	return ret;
}

bool RedisString::incr(const char* key, long long int* result /*= NULL*/)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.incr(key, result);
}

bool RedisString::incrby(const char* key, long long int inc, long long int* result /*= NULL*/)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.incrby(key, inc, result);
}

bool RedisString::incrbyfloat(const char* key, double inc, double* result /*= NULL*/)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.incrbyfloat(key, inc, result);
}

bool RedisString::decr(const char* key, long long int* result /*= NULL*/)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.decr(key, result);
}

bool RedisString::decrby(const char* key, long long int dec, long long int* result /*= NULL*/)
{
	acl::redis_string cmd_string;
	init_redis_string(&cmd_string);

	return cmd_string.decrby(key, dec, result);
}
