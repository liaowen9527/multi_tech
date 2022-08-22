#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "MongoDBResult.h"

class LWUtil3rd_API MongoDBCollection
{
public:
	MongoDBCollection();
	~MongoDBCollection();

public:
	MongoDBCursor Find(const LWJson& q);
	MongoDBResult<LWJson> FindOne(const LWJson& q);

	MongoDBResult<LWJson> Insert(const std::vector<LWJson>& docs);

	void DeleteMany(const LWJson& q);
	void DeleteOne(const LWJson& q);

	void UpdateMany(const LWJson& q, const LWJson& docUpdate, bool bUpset);
	void UpdateOne(const LWJson& q, const LWJson& docUpdate, bool bUpset);

	MongoDBCursor Aggregate(const LWJson& pipeline);

	void BulkWrite(const LWJson& requests);
};

