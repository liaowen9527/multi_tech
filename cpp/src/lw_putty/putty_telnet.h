#pragma once
#include <string>
#include <memory>
#include "lw_putty_dll.h"
#include "putty_client.h"

class LWPutty_API PuttyTelnet : public PuttyClient
{
public:
	PuttyTelnet();
	virtual ~PuttyTelnet();

public:
	virtual bool Connect(const std::string& strRemoteHost, int nPort = 23);

public:


};

typedef std::shared_ptr<PuttyTelnet> PuttyTelnetPtr;
