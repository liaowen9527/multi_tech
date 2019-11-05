#pragma once
#include "netkit.h"
#include "tcp_client.h"

namespace netkit
{
	class TcpClientAsio
	{
	public:
		TcpClientAsio();
		~TcpClientAsio();

	protected:
		boost::asio::io_service m_ios;
		boost::asio::io_service m_wks;
		boost::shared_ptr<boost::asio::ssl::context> m_sslCtx;
	};
}