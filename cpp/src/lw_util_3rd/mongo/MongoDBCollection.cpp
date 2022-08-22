#include "MongoDBCollection.h"

MongoDBCollection::MongoDBCollection()
{

}

MongoDBCollection::~MongoDBCollection()
{

}

MongoDBCursor MongoDBCollection::Find(const LWJson& q)
{
	MongoDBCursor ret;
	try
	{

	}
	catch (const std::exception& e)
	{
		ret.GetErrCode().SetErrorCode(LW_MONGO_UNKNOWN_ERROR_CODE, e.what());
	}
	catch (...)
	{

	}

	return ret;
}

MongoDBResult<LWJson> MongoDBCollection::FindOne(const LWJson& q)
{
	MongoDBResult<LWJson> ret;
	try
	{

	}
	catch (const std::exception& e)
	{
		ret.GetErrCode().SetErrorCode(LW_MONGO_UNKNOWN_ERROR_CODE, e.what());
	}
	catch (...)
	{

	}

	return ret;
}

MongoDBResult<LWJson> MongoDBCollection::Insert(const std::vector<LWJson>& docs)
{
	MongoDBResult<LWJson> ret;
	try
	{

	}
	catch (const std::exception& e)
	{
		ret.GetErrCode().SetErrorCode(LW_MONGO_UNKNOWN_ERROR_CODE, e.what());
	}
	catch (...)
	{

	}

	return ret;
}

void MongoDBCollection::DeleteMany(const LWJson& q)
{

}

void MongoDBCollection::DeleteOne(const LWJson& q)
{

}

void MongoDBCollection::UpdateMany(const LWJson& q, const LWJson& docUpdate, bool bUpset)
{

}

void MongoDBCollection::UpdateOne(const LWJson& q, const LWJson& docUpdate, bool bUpset)
{

}

MongoDBCursor MongoDBCollection::Aggregate(const LWJson& pipeline)
{
	MongoDBCursor ret;
	try
	{

	}
	catch (const std::exception& e)
	{
		ret.GetErrCode().SetErrorCode(LW_MONGO_UNKNOWN_ERROR_CODE, e.what());
	}
	catch (...)
	{

	}

	return ret;
}

void MongoDBCollection::BulkWrite(const LWJson& requests)
{

}
