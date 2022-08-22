#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "MongoDBUri.h"
#include "json/LWJson.h"

typedef struct _mongoc_client_pool_t mongoc_client_pool_t;

class MongoDBClientPool
{
public:
	MongoDBClientPool(const MongoDBUri& uri);
	~MongoDBClientPool();

public:
	void SetMaxSize(int nMaxCount);

private:
	mongoc_client_pool_t* m_pool;
};

