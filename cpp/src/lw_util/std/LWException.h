
#pragma once

#include "lw_util.h"
#include <string>

#ifndef LW_EXCEPTION_DEFAULT_ERROR_CODE
#define LW_EXCEPTION_DEFAULT_ERROR_CODE -9999
#endif

class LWUtil_API LWException : public std::runtime_error
{
public:
	typedef std::runtime_error _Mybase;

	LWException();

	explicit LWException(const std::string& _Message);

	explicit LWException(const char *_Message);

	explicit LWException(int nErrCode, const std::string& _Message);

	explicit LWException(int nErrCode, const char *_Message);

	explicit operator bool() const noexcept
	{
		return bool(m_errCode);
	}

	int GetErrCode() const;

	void SetErrorCode(int nErrCode, const char *_Message);

private:
	int m_errCode;
};

