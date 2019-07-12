#include "destination.h"

namespace lw_live {

	
	Destination::Destination()
	{

	}

	Destination::~Destination()
	{

	}

	void Destination::GetClientParam() const
	{
		return m_clientParam;
	}

	void Destination::SetClientParam(ClientParamPtr clientParam)
	{
		m_clientParam = clientParam;
	}

	std::string Destination::GetLoginUserName() const
	{
		return "";
	}

	std::string Destination::GetLoginPassword() const
	{
		return "";
	}

	std::string Destination::GetEnableUserName() const
	{
		return "";
	}

	std::string Destination::GetEnablePassword() const
	{
		return "";
	}

	bool Destination::IsAutoLogin() const
	{
		return true;
	}

}


