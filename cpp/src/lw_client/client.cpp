#include "client.h"

namespace lw_client {

	Client::Client()
		: m_pNotify(new NotifyEx)
	{

	}

	Client::~Client()
	{

	}

	void Client::SetNotify(Notify* pNotify)
	{
		m_pNotify->SetNotify(pNotify);
	}

	void Client::SetTermSize(int nWidth, int nHeight)
	{

	}

	bool Client::IsConnected()
	{
		return m_connStatus.is_connected();
	}

	bool Client::Connect(Parameter* pConnParam)
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

	bool Client::AsyncConnect(Parameter* pConnParam)
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