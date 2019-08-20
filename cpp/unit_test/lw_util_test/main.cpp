#include "TestThread.hpp"
#include "mongo/MongoDBClient.h"

int main(void)
{
// 	TestThread test;
// 	test.Test();

	MongoDBClient::Initialize();

	MongoUri uri("127.0.0.1");

	MongoDBClient client;
	client.ConnectTo(uri.to_string());

	
}