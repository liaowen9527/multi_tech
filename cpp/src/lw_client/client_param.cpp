#include "client_param.h"
#include <iostream>

namespace lw_client {

	ClientParam::ClientParam(const std::string& host, int port)
		: m_host(host)
		, m_port(port)
		, m_bLoginConfirm(true)
	{

	}

	ClientParam::~ClientParam()
	{

	}

	std::string ClientParam::GetHost() const
	{
		return m_host;
	}

	void ClientParam::SetHost(const std::string& str)
	{
		m_host = str;
	}

	int ClientParam::GetPort() const
	{
		return m_port;
	}

	void ClientParam::SetPort(int port)
	{
		m_port = port;
	}

	std::string ClientParam::GetUsername() const
	{
		return m_username;
	}

	void ClientParam::SetUsername(const std::string& str)
	{
		m_username = str;
	}

	std::string ClientParam::GetPassword() const
	{
		return m_password;
	}

	void ClientParam::SetPassword(const std::string& str)
	{
		m_password = str;
	}

	bool ClientParam::IsConfirmLogin()
	{
		return m_bLoginConfirm;
	}

	void ClientParam::SetLoginConfirm(bool bConfirm)
	{
		m_bLoginConfirm = bConfirm;
	}

}






