#pragma once
#include <string>
#include <vector>
#include <set>
#include "xson/xson.h"
#include "xson/xson_builder.h"
#include "lw_util_3rd.h"

class LWUtil3rd_API MongoQuery
{
public:
	template<class T>
	static xson Equal(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document(strField, val);

		return builder.extract();
	}

	template<class T>
	static xson NotEqual(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document(strField, [&](xson_builder& sub) {
			sub.append("$ne", val);
		});

		return builder.extract();
	}

	static xson And(const xson& val, const xson& val2);
	static xson And(const std::vector<xson>& vec);

	static xson Or(const xson& val, const xson& val2);
	static xson Or(const std::vector<xson>& vec);

	static xson Exist(const std::string& strField, bool isExist = true);

	template<class T>
	static xson In(const std::string& strField, const std::set<T>& setVal)
	{
		xson_builder builder;
		builder.append_document(strField, [&](xson_builder& sub) {
			sub.append_array("$in", [&](xson_builder& sub2) {
				for (const T& val : setVal)
				{
					sub2.append(val);
				}
			});
		});

		return builder.extract();
	}
	static xson In(const std::string& strField, const xson& arrVal);

	template<class T>
	static xson GreaterThan(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append(strField, [&](xson_builder& sub) {
			sub.append("$gt", val);
		});

		return builder.extract();
	}

	template<class T>
	static xson GreaterEqual(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document(strField, [&](xson_builder& sub) {
			sub.append("$gte", val);
		});

		return builder.extract();
	}

	template<class T>
	static xson LittleThan(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document(strField, [&](xson_builder& sub) {
			sub.append("$lt", val);
		});

		return builder.extract();
	}

	template<class T>
	static xson LittleEqual(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document(strField, [&](xson_builder& sub) {
			sub.append("$lte", val);
		});

		return builder.extract();
	}

	template<class T>
	static xson BetweenTime(const std::string& strField, time_t begin, time_t end)
	{
		xson geQry = GreaterEqual(strField, begin);
		xson leQry = LittleEqual(strField, end);

		if (0 != begin && 0 != end)
		{
			return And(geQry, leQry);
		}
		else if (0 != begin)
		{
			return geQry;
		}
		else
		{
			return leQry;
		}

		return xson();
	}

	static xson All();
};


class MongoUpdate
{
public:
	template<class T>
	static xson UpsetField(const std::string& strField, T val)
	{
		xson_builder builder;
		builder.append_document("$set", [&](xson_builder& sub) {
			sub.append(strField, val);
		});

		return builder.extract();
	}

	static xson DeleteField(const std::string& strField);

	static xson UpdateLastTime(const std::string& strField);

public:
	class Array
	{
	public:
		template<class T>
		static xson Push(const std::string& strArrayField, T val)
		{
			xson_builder builder;
			builder.append_document("$push", [&](xson_builder& sub) {
				sub.append(strArrayField, val);
			});

			return builder.extract();
		}

		static xson Pop(const std::string& strArrayField);
		static xson PopFront(const std::string& strArrayField);
		static xson Remove(const std::string& strArrayField, const xson& query);
	};
};