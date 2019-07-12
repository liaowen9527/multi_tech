#include "client.h"

namespace lw_client {

	Client::Client()
	{

	}

	Client::~Client()
	{

	}

	ClientHandlerPtr Client::GetHandler()
	{
		return m_handler;
	}

	void Client::SetHandler(ClientHandlerPtr handle)
	{
		m_handler = handle;
	}

	bool Client::IsConnected()
	{
		return m_connStatus.is_connected();
	}

	bool Client::Connect(ClientParam* pConnParam)
	{
		if (!AsyncConnect(pConnParam))
		{
			m_connStatus.disconnected();
			return false;
		}

		if (!m_connStatus.wait_connected())
		{
			return false;
		}

		return true;
	}

	bool Client::AsyncConnect(ClientParam* pConnParam)
	{
		m_connStatus.connecting();
		return true;
	}

	bool Client::DisConnect()
	{
		if (!AsyncDisConnect())
		{
			return false;
		}

		return m_connStatus.wait_disconnected();
	}

	bool Client::AsyncDisConnect()
	{
		if (m_connStatus.is_connected())
		{
			m_connStatus.disconnecting();
		}
		else
		{
			m_connStatus.disconnected();
		}

		return true;
	}



}