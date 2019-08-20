// mongocxx-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "mongo/MongoDBClient.h"
#include "xson/xson_builder.h"
#include "mongo/MongoQuery.h"

int main()
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

	MongoDBClient::Uninitialize();
}
