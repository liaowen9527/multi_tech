#include "MongoDBClient.h"
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/logic_error.hpp>

#include "PlatformMacros.h"
#include "xson/xson_bson.h"


#define get_client(ptr) (*(mongocxx::client*)(ptr.get()))
#define get_collection(ptr) (*(mongocxx::collection*)(ptr.get()))


MongoDBClient::MongoDBClient()
	: m_conn(nullptr)
{
}

MongoDBClient::~MongoDBClient()
{
}

static mongocxx::instance* s_inst = nullptr;
bool MongoDBClient::Initialize()
{
	try
	{
		if (nullptr == s_inst)
		{
			s_inst = new mongocxx::instance{};
		}

		return true;
	}
	catch (const mongocxx::logic_error& e)
	{
	}
	catch (...)
	{

	}

	SAFE_DELETE(s_inst);

	return false;
}

void MongoDBClient::Uninitialize()
{
	SAFE_DELETE(s_inst);
}

bool MongoDBClient::ConnectTo(const MongoUri& svr)
{
	return ConnectTo(svr.to_string());
}

bool MongoDBClient::ConnectTo(const std::string& connStr /*= "mongodb://localhost:27017"*/)
{
	try
	{
		auto* p = new mongocxx::client{ mongocxx::uri{connStr} };
		SetClient(std::shared_ptr<void>(p));
		
		return true;
	}
	catch (const mongocxx::exception& e)
	{
	}
	catch (...)
	{

	}

	return false;
}

bool MongoDBClient::GetCollection(const std::string& strDB, const std::string& strCollection, MongoDBCollection& collection)
{
	auto conn = GetClient();
	if (nullptr == conn)
	{
		return false;
	}

	try
	{
		mongocxx::client& client = get_client(conn);

		mongocxx::collection* pCollection = new mongocxx::collection();
		(*pCollection) = client[strDB][strCollection];

		collection.SetCollection(std::shared_ptr<void>(pCollection));

		return true;
	}
	catch (const mongocxx::exception& e)
	{
	}
	catch (...)
	{

	}

	return false;
}

std::shared_ptr<void> MongoDBClient::GetClient()
{
	return m_conn;
}

void MongoDBClient::SetClient(std::shared_ptr<void> ptr)
{
	m_conn = ptr;
}

MongoDBCollection::MongoDBCollection(std::shared_ptr<void> colletion/* = nullptr*/)
	: m_collection(colletion)
{

}

MongoDBCollection::~MongoDBCollection()
{

}

bool MongoDBCollection::Find(const xson_view& query, std::function<void(xson_view&)> parse_doc)
{
	auto temp = GetCollection();
	if (nullptr == temp)
	{
		return false;
	}

	mongocxx::collection& collection = get_collection(temp);
	auto cursor = collection.find(to_bsonview(query));
	for (auto&& doc : cursor)
	{
		xson_view temp = to_xsonview(doc);
		parse_doc(temp);
	}

	return false;
}

bool MongoDBCollection::FindOne(const xson_view& query, xson& doc)
{
	auto temp = GetCollection();
	if (nullptr == temp)
	{
		return false;
	}

	mongocxx::collection& collection = get_collection(temp);

	auto bsondoc = collection.find_one(to_bsonview(query));
	if (!bsondoc)
	{
		return false;
	}

	doc = to_xson(bsondoc.get());
	return true;
}

bool MongoDBCollection::InsertMany(const std::vector<xson>& docs)
{
	return false;
}

bool MongoDBCollection::InsertOne(const xson& doc)
{
	auto temp = GetCollection();
	if (nullptr == temp)
	{
		return false;
	}

	//mongocxx::collection& collection = get_collection(temp);

	//collection->insert_one(xson2bson(query));

	return true;
}

bool MongoDBCollection::DeleteMany(const xson& query)
{
	return false;
}

bool MongoDBCollection::DeleteOne(const xson& query)
{
	return false;
}

std::shared_ptr<void> MongoDBCollection::GetCollection()
{
	return m_collection;
}

void MongoDBCollection::SetCollection(std::shared_ptr<void> colletion)
{
	m_collection = colletion;
}
