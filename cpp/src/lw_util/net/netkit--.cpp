#include "stdafx.h"
#include "log/easylog.h"
#include "netkit.h"
#include "boost/uuid/uuid_io.hpp"
#ifdef WIN32
#include <io.h>
#endif

namespace netkit
{
	bool ConnectResult::Succ()
	{
		return ec && *ec ? false : true;
	}

	bool WriteResult::Succ()
	{
		return ec && *ec ? false : true;
	}

	ClientConfig::ClientConfig()
	{
		ssl = false;
		port = 0;
		ioThdCnt = 1;
		wkThdCnt = 1;
		sslVerifyCert = false;
		sslHandshakeTimeout = 5;
		tcpKeepaliveEnable = false;
		tcpKeepaliveTimeout = 0;
		tcpKeepaliveInterval = 0;
		tcpKeepaliveCount = 0;
	}

	bool ClientConfig::CheckValid(std::string &errMsg) const
	{
		if (proto != "tcp")
		{
			errMsg = "invalid protocol type";
			return false;
		}

		if (svrAddr.empty())
		{
			errMsg = "invalid address";
			return false;
		}

		if (port == 0)
		{
			errMsg = "invalid port";
			return false;
		}

		if (ssl)
		{
			if (sslVerifyCert)
			{
				if (!sslCertFile.empty() && access(sslCertFile.c_str(), 4) == -1)
				{
					errMsg = "unreadable ssl certificate file";
					return false;
				}
			}

			if (sslHandshakeTimeout < 1)
			{
				errMsg = "invalid ssl handshake timeout value";
				return false;
			}
		}

		if (!pkHandler)
		{
			errMsg = "no packet handler";
			return false;
		}

		return true;
	}

	Endpoint::Endpoint()
		: port(0)
	{
	}

	bool Endpoint::CheckValid(std::string &errMsg) const
	{
		if (addr.empty())
		{
			errMsg = "invalid address";
			return false;
		}

		if (port == 0)
		{
			errMsg = "invalid port";
			return false;
		}

		return true;
	}

	ServerConfig::ServerConfig()
	{
		this->ssl = false;
		this->port = 0;
		this->ioThdCnt = 1;
		this->wkThdCnt = 1;
		this->sslHandshakeTimeout = 5;
		this->bUseInternalUnzip = true;
		this->reuseAddr = false;
	}

	bool ServerConfig::CheckValid(std::string &errMsg) const
	{
		if (proto != "tcp")
		{
			errMsg = "invalid protocol type";
			return false;
		}

		if (addr.empty())
		{
			errMsg = "invalid address";
			return false;
		}

		if (port == 0)
		{
			errMsg = "invalid port";
			return false;
		}

		if (ssl)
		{
			if (sslCertFile.empty())
			{
				errMsg = "no ssl certificate file";
				return false;
			}

			if (access(sslCertFile.c_str(), 4) == -1)
			{
				errMsg = "unreadable ssl certificate file";
				return false;
			}

			if (sslKeyFile.empty())
			{
				errMsg = "no ssl key file";
				return false;
			}

			if (access(sslKeyFile.c_str(), 4) == -1)
			{
				errMsg = "unreadable ssl key file";
				return false;
			}

			if (sslHandshakeTimeout < 1)
			{
				errMsg = "invalid ssl handshake timeout value";
				return false;
			}
		}

		if (!pkHandler)
		{
			errMsg = "no packet handler";
			return false;
		}

		return true;
	}

	ClientStatistics::ClientStatistics()
		: unsentPackCount(0)
		, unsentDataSize(0)
		, sentDataSize(0)
		, nSentDataSizeRate(0)
	{
	}

	time_t Connection::GetLastRequestTime()
	{
		return 0;
	}

	void Connection::SetLastRequestTime(time_t tim)
	{
	}

	bool Connection::GetTrustful()
	{
		return false;
	}

	void Connection::SetTrustful(bool trustful)
	{
	}


	//////////////////////////////////////////////
	void ConnectionManager::Add(const std::string &id, ConnectionPtr conn)
	{
		m_conns.append(id, conn);
	}

	void ConnectionManager::Remove(ConnectionPtr conn)
	{
		m_conns.remove_if([conn](const std::string& key, const ConnectionPtr& value) {
			return conn == value;
		}, true);
	}

	void ConnectionManager::Remove(const std::string &id)
	{
		m_conns.remove(id);
	}

	bool ConnectionManager::Find(const std::string &id, ConnectionPtr &conn)
	{
		return m_conns.get(id, conn);
	}

	bool ConnectionManager::Find(const ConnectionPtr &conn, std::string &id)
	{
		m_conns.for_each([conn, &id](const std::string& key, const ConnectionPtr& value, bool& next) {
			if (value == conn)
			{
				next = false;
				id = key;
			}
		});

		return id.empty();
	}

	void ConnectionManager::GetAll(std::vector<ConnectionPtr> &conns)
	{
		m_conns.for_each([&conns](const std::string& key, const ConnectionPtr& value, bool& next) {
			conns.push_back(value);
		});
	}

	void ConnectionManager::RemoveAll()
	{
		m_conns.clear();
	}

	size_t ConnectionManager::Count()
	{
		return m_conns.size();
	}
	
	/////////////////////////////////////////////////////////

	ClientPtr Factory::CreateClient(const ClientConfig &cfg)
	{
		std::string errMsg;
		if (!cfg.CheckValid(errMsg))
		{
			EasyLog(InstNet, LOG_ERROR, "invalid config, %s", errMsg.c_str());
			return nullptr;
		}

		if (cfg.proto == "tcp")
		{
			//return TcpClientPtr(new TcpClient(cfg));
		}

		return nullptr;
	}

	netkit::ServerPtr Factory::CreateServer(const ServerConfig &cfg)
	{
		std::string errMsg;
		if (!cfg.CheckValid(errMsg))
		{
			EasyLog(InstNet, LOG_ERROR, "invalid config, %s", errMsg.c_str());
			return nullptr;
		}

		if (cfg.proto == "tcp")
		{
			//return TcpServerPtr(new TcpServer(cfg));
		}

		return nullptr;
	}

}
