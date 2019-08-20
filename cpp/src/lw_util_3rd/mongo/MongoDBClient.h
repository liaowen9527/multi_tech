#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "MongoUri.h"
#include "xson/xson.h"

class MongoDBCollection;

//if you want connect to mongodb, you must have called mongo::client::initialize();
class LWUtil3rd_API MongoDBClient
{
public:
	MongoDBClient();
	~MongoDBClient();

public:
	static bool Initialize();
	static void Uninitialize();

public:
	bool ConnectTo(const MongoUri& svr);
	bool ConnectTo(const std::string& connStr = "mongodb://localhost:27017");

	bool GetCollection(const std::string& strDB, const std::string& strCollection, MongoDBCollection& collection);

protected:
	std::shared_ptr<void> GetClient();
	void SetClient(std::shared_ptr<void> ptr);

protected:
	std::shared_ptr<void> m_conn;
};


class LWUtil3rd_API MongoDBCollection
{
	friend MongoDBClient;
public:
	MongoDBCollection(std::shared_ptr<void> colletion = nullptr);
	~MongoDBCollection();

public:
	bool Find(const xson_view& query, std::function<void(xson_view&)> parse_doc);
	bool FindOne(const xson_view& query, xson& doc);

	bool InsertMany(const std::vector<xson>& docs);
	bool InsertOne(const xson& doc);

	bool DeleteMany(const xson& query);
	bool DeleteOne(const xson& query);

protected:
	std::shared_ptr<void> GetCollection();
	void SetCollection(std::shared_ptr<void> colletion);

protected:
	std::shared_ptr<void> m_collection;
};
