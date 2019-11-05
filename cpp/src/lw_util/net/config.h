#pragma once
#include "define.h"

namespace netkit {

	struct NetkitApi ClientConfig
	{
		std::string proto;					// tcp or websocket
		std::string svrAddr;				// server ip address
		unsigned short port;				// server port
		bool ssl;								// use ssl or not, default not
		bool sslVerifyCert;				// ssl verify certificate or not, default not
		std::string sslCertFile;			// ssl verify certificate file path
		size_t sslHandshakeTimeout;	// ssl handshake timeout in seconds
		size_t ioThdCnt;					// io thread count, default 1
		size_t wkThdCnt;					// packet handle thread count, default 1
		PackHandlerPtr pkHandler;		// packet handler
		bool tcpKeepaliveEnable;		// tcp keep alive enable
		int tcpKeepaliveTimeout;		// timeout of tcp keep alive
		int tcpKeepaliveInterval;		// interval of tcp keep alive
		int tcpKeepaliveCount;			// count of tcp keep alive

	};


	struct NetkitApi ServerConfig
	{
		std::string proto;					// tcp or websocket
		std::string addr;					// ip address to listen
		unsigned short port;				// port to listen
		bool ssl;								// use ssl or not, default not
		std::string sslCertFile;			// ssl certificate file path
		std::string sslKeyFile;			// ssl key file path
		size_t sslHandshakeTimeout;	// ssl handshake timeout in seconds
		size_t ioThdCnt;					// io thread count, default 1
		size_t wkThdCnt;					// packet handle thread count, default 1
		PackHandlerPtr pkHandler;		// packet handler
		bool bUseInternalUnzip;			// 
		bool reuseAddr;					// reuse bind addr

	};


}

