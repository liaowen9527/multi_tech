#include "LWException.h"



LWException::LWException() : _Mybase("")
, m_errCode(0)
{

}

LWException::LWException(const std::string& _Message) : _Mybase(_Message.c_str())
{
	// construct from message string
	m_errCode = _Message.empty() ? 0 : LW_EXCEPTION_DEFAULT_ERROR_CODE;
}

LWException::LWException(const char *_Message) : _Mybase(_Message)
{
	// construct from message string
	if (nullptr == _Message || strlen(_Message) == 0)
	{
		m_errCode = 0;
	}
	else
	{
		m_errCode = LW_EXCEPTION_DEFAULT_ERROR_CODE;
	}
}

LWException::LWException(int nErrCode, const std::string& _Message) : _Mybase(_Message.c_str())
, m_errCode(nErrCode)
{
	// construct from message string
}

LWException::LWException(int nErrCode, const char *_Message) : _Mybase(_Message)
, m_errCode(nErrCode)
{
	// construct from message string
	if (nullptr == _Message || strlen(_Message) == 0)
	{
		m_errCode = 0;
	}
	else
	{
		m_errCode = LW_EXCEPTION_DEFAULT_ERROR_CODE;
	}
}

int LWException::GetErrCode() const
{
	return m_errCode;
}

void LWException::SetErrorCode(int nErrCode, const char *_Message)
{
	std::exception* pException = (std::exception*)this;
	*pException = std::exception(_Message);

	m_errCode = nErrCode;
}
