// mongocxx-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <time.h>
#include "mongo/MongoDBClient.h"
#include "xson/xson_builder.h"
#include "mongo/MongoQuery.h"

#include <stdlib.h>
#include <stdio.h>
#include "redis/RedisClient.h"
#include "redis/RedisClientCluster.h"


void test_mongo()
{
	MongoDBClient::Initialize();
	MongoDBClient client;
	client.ConnectTo();

	MongoDBCollection collection;
	client.GetCollection("testdb", "testcollection", collection);

	auto func = [&](xson_builder& builder) {
		builder.append("$exists", true);
	};
	/*xson_builder builder;
	builder.append_document("date", [&](xson_builder& builder) {
		builder.append("$exists", true);
	});*/

	std::set<std::string> setHello;
	//setHello.insert("world");
	setHello.insert("aaa");
	collection.Find(MongoQuery::In("hello", setHello).view(), [](xson_view& view) {
		std::cout << view.to_string().c_str() << std::endl;

		view.get_elements([](xson_element& ele) {
			//ele.output();
		});

	});

	xson_builder builder;
	builder.append("lev", std::string("debug"));
	builder.append("local", time(0) * 1000);
	builder.append("pid", 123);
	builder.append("tid", 456);
	builder.append("func", "main");
	builder.append("log", "aaaa");
	builder.append_array("lable", [&](xson_builder& sub) {
		sub.append("liaowen");
		sub.append(time(0) * 1000);
	});
	builder.append_document("timeline", [&](xson_builder& sub) {
		sub.append("$currentDate", true);
	});

	collection.InsertOne(builder.view());

	MongoDBClient::Uninitialize();
}

void test_redis()
{
	// init socket module for windows
	RedisClient::InitEnv();

	const char* redis_addr = "10.10.0.130:6379";
	int conn_timeout = 10, rw_timeout = 10;

	// the redis client connection
	//RedisClient client(redis_addr, conn_timeout, rw_timeout);
	//client.set_password("netbrain");
	RedisClientCluster client;
	client.set(redis_addr, conn_timeout, rw_timeout);
	client.set_password(redis_addr, "netbrain");

	const char* key = "test_key";

	// test redis STRING command
	// bind redis_string command with redis connection
	if (client.String().set(key, "test_value"))
	{
		std::string strValue;
		if (client.String().get(key, strValue))
		{
			std::cout << "redis value:" << strValue.c_str() << std::endl;
		}
	}
}

int main()
{
	test_redis();
}