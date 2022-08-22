#include "MongoDBClientPool.h"
#include <iostream>
#include <mongoc/mongoc.h>

MongoDBClientPool::MongoDBClientPool(const MongoDBUri& uri)
{
	m_pool = mongoc_client_pool_new(uri.GetMongocUri());
}

MongoDBClientPool::~MongoDBClientPool()
{
	if (m_pool)
	{
		mongoc_client_pool_destroy(m_pool);
	}
}

void MongoDBClientPool::SetMaxSize(int nMaxCount)
{
	mongoc_client_pool_max_size(m_pool, nMaxCount);
}

