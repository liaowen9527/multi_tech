#pragma once
#include <string>
#include <functional>
#include "lw_util_3rd.h"
#include "json/LWJson.h"

template<class Results>
class LWUtil3rd_API MongoDBResult
{
public:
	explicit operator bool() const noexcept
	{
		return bool(m_ec);
	}

private:
	Results m_result;
	std::error_code m_ec;
};

