#include "MongoDBManager.h"

MongoDBManager::MongoDBManager()
{

}

MongoDBManager::~MongoDBManager()
{

}

MongoDBManager* MongoDBManager::Instance()
{
	static MongoDBManager* s_pInst = nullptr;
	if (nullptr == s_pInst)
	{
		s_pInst = new MongoDBManager();
	}

	return s_pInst;
}
