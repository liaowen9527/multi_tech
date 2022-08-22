#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "MongoDBUri.h"
#include "json/LWJson.h"

//if you want connect to mongodb, you must have called mongo::client::initialize();
class MongoDBClient
{
public:
	MongoDBClient();
	~MongoDBClient();

public:
	bool ConnectTo(const MongoDBUri& uri);



};
typedef std::shared_ptr<MongoDBClient> MongoDBClientPtr;


class MongoDBClientPool
{
public:
	MongoDBClientPool();
	~MongoDBClientPool();

private:

};

