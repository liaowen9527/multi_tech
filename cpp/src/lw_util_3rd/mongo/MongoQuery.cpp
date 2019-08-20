#include "MongoQuery.h"
#include <sstream>


xson MongoQuery::And(const xson& val, const xson& val2)
{
	xson_builder builder;
	builder.append_array("$and", [&](xson_builder& sub) {
		sub.append(val);
		sub.append(val2);
	});

	return builder.extract();
}

xson MongoQuery::And(const std::vector<xson>& vec)
{
	xson_builder builder;
	builder.append_array("$and", [&](xson_builder& sub) {
		for (const xson& temp : vec)
		{
			sub.append(temp);
		}
	});

	return builder.extract();
}

xson MongoQuery::Or(const xson& val, const xson& val2)
{
	xson_builder builder;
	builder.append_array("$or", [&](xson_builder& sub) {
		sub.append(val);
		sub.append(val2);
	});

	return builder.extract();
}

xson MongoQuery::Or(const std::vector<xson>& vec)
{
	xson_builder builder;
	builder.append_array("$or", [&](xson_builder& sub) {
		for (const xson& temp : vec)
		{
			sub.append(temp);
		}
	});

	return builder.extract();
}

xson MongoQuery::Exist(const std::string& strField, bool isExist/* = true*/)
{
	xson_builder builder;
	builder.append_document(strField, [&](xson_builder& sub) {
		sub.append("$exists", isExist);
	});

	return builder.extract();
}


xson MongoQuery::In(const std::string& strField, const xson& arrVal)
{
	xson_builder builder;
	builder.append_document(strField, [&](xson_builder& sub) {
		sub.append("$in", arrVal);
	});

	return builder.extract();
}

xson MongoQuery::All()
{
	return xson();
}

xson MongoUpdate::DeleteField(const std::string& strField)
{
	xson_builder builder;
	builder.append_document("$unset", [&](xson_builder& sub) {
		sub.append(strField, true);
	});

	return builder.extract();
}


xson MongoUpdate::UpdateLastTime(const std::string& strField)
{
	xson_builder builder;
	builder.append_document("$currentDate", [&](xson_builder& sub) {
		sub.append(strField, true);
	});

	return builder.extract();
}


xson MongoUpdate::Array::Pop(const std::string& strArrayField)
{
	xson_builder builder;
	builder.append_document("$pop", [&](xson_builder& sub) {
		sub.append(strArrayField, 1);
	});

	return builder.extract();
}

xson MongoUpdate::Array::PopFront(const std::string& strArrayField)
{
	xson_builder builder;
	builder.append_document("$pop", [&](xson_builder& sub) {
		sub.append(strArrayField, -1);
	});

	return builder.extract();
}

xson MongoUpdate::Array::Remove(const std::string& strArrayField, const xson& query)
{
	xson_builder builder;
	builder.append_document("$pull", [&](xson_builder& sub) {
		sub.append(strArrayField, query);
	});

	return builder.extract();
}

