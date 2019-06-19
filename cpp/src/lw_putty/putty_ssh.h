#pragma once
#include <string>
#include <memory>
#include "lw_putty_dll.h"
#include "putty_client.h"

enum EnumSshVer
{
	ssh_v1 = 0,
	ssh_v2
};

class LWPutty_API PuttySsh : public PuttyClient
{
public:
	PuttySsh();
	virtual ~PuttySsh();

public:
	virtual bool Connect(const std::string& strRemoteHost, int nPort = 22);

public:
	void SetSshVer(EnumSshVer ver);

	void SetKeyfile(const std::string& strKeyFile);

protected:
	virtual int from_backend(int is_stderr, const char *data, int len);
};

typedef std::shared_ptr<PuttySsh> PuttySshPtr;
