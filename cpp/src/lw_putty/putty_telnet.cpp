#include "putty_telnet.h"
#include "putty_conf.h"

PuttyTelnet::PuttyTelnet()
{
	SetTelnetMode(conf);
}

PuttyTelnet::~PuttyTelnet()
{

}

bool PuttyTelnet::Connect(const std::string& strRemoteHost, int nPort /*= 23*/)
{
	return PuttyClient::Connect(strRemoteHost, nPort);
}




