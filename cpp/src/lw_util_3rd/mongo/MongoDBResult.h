#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "json/LWJson.h"
#include "std/LWErrorCode.h"

#define LW_MONGO_UNKNOWN_ERROR_CODE -1024

template<class Results>
class LWUtil3rd_API MongoDBResult
{
public:
	explicit operator bool() const noexcept
	{
		return bool(m_ec);
	}

	LWErrorCode& GetErrCode()
	{
		return m_ec;
	}

	Results& GetResult()
	{
		if (m_ec)
		{
			throw m_ec;
		}

		return m_result;
	}

private:
	Results m_result;
	LWErrorCode m_ec;
};

class LWUtil3rd_API MongoDBCursor
{
public:
	MongoDBCursor();
	~MongoDBCursor();

public:
	explicit operator bool() const noexcept
	{
		return bool(m_ec);
	}

	LWErrorCode& GetErrCode()
	{
		return m_ec;
	}

private:
	LWErrorCode m_ec;
};
