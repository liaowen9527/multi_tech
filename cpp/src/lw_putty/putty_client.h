#pragma once
#include <string>
#include <winsock2.h> 
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include "lw_putty_dll.h"

typedef struct conf_tag Conf;
typedef struct backend_tag Backend;
class Connector;

class LWPutty_API PuttyClientNotify
{
public:
	virtual void OnConnected() = 0;
	virtual void OnDisconnected(int nErrCode, const std::string& err_msg) = 0;

	virtual void OnReceived(const char* str, int nLen) = 0;

	virtual void OnError(int nErrCode, const std::string& err_msg) = 0;

	virtual void OnLoginResult(bool bValue) {};
};

enum EnumSpecialStep
{
	emNoSpecial = -1,
	emUserName,
	emPassword,
	emAskInput
};

class LWPutty_API PuttyClient
{
	friend Connector;
public:
	PuttyClient();
	virtual ~PuttyClient();

	PuttyClientNotify* GetNotify();
	void SetNotify(PuttyClientNotify* pNotify);

public:
	static bool InitEnv();

public:
	bool IsConnected();
	virtual bool Connect(const std::string& strRemoteHost, int nPort = 23);
	virtual bool DisConnect();

	virtual bool Send(const std::string& data);

	void GetTermSize(int width, int height);
	void SetTermSize(int width, int height);

public:
	virtual std::string GetUserName();
	virtual void SetUserName(const std::string& strUserName);

	virtual std::string GetPassword();
	virtual void SetPassword(const std::string& strPassword);

	bool IsConfirmLogin();
	void SetLoginConfirm(bool bConfirm);

	void SetUserNameInvalid();
	void SetPasswordInvalid();

	std::string GetInputBuffer();
	void SetInputBuffer(const std::string& str);

	void WaitForInput(const std::string& strPrompt, EnumSpecialStep emStep);
	void WaitForUserName(const std::string& strPrompt);
	void WaitForPassword(const std::string& strPrompt);

	std::string NormalizeEOL(const std::string &text, const std::string &eof);
	void OutputMessage(int err_code, const std::string& str);

	std::string& GetBuffer_unsafe(EnumSpecialStep emStep, bool& bDisplay);

public:
	//callback from putty source
	virtual int from_backend(int is_stderr, const char *data, int len);
	void connection_fatal(void *frontend, const std::string& err_msg);

protected:
	bool Init();
	void FixupRemoteHost();
	bool InitConnParams();
	void InitLogCtx();
	bool StartUpConn();
	void CleanConn();

	void SetWSAEvent(SOCKET skt, WSAEVENT netevent);

protected:
	bool IsConnected_unsafe();

	virtual bool Send_unsafe(const std::string& data);

protected:
	bool StartUpRecvThread();
	void FreeRecvThread();
	void InnerThread();

protected:
	bool DoEvent();
	void DoInputEvent();
	void DoRecvData();
	void NotifyRecvData();

	bool TryRspSelf(const std::string& data);
	
	// Method:    input string into buffer
	// Returns:   return true if the buffer has completed
	bool AppendInput(std::string& strBuffer, const std::string& strAppend, bool bDisplay);

#pragma region var_from_putty

protected:
	Conf *conf;
 	void *backhandle;
 	Backend *back;

protected:
	SOCKET *sklist;
	int skcount, sksize;
	bool use_subsystem = 0;

#pragma endregion var_from_putty

protected:
	SOCKET m_socket;
	WSAEVENT m_netEvent;
	HANDLE m_inputEvent;

	PuttyClientNotify* m_pNotify;
	std::thread* m_pEventThread;
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	
	std::queue<std::string> m_msgQueue;
	std::queue<std::string> m_recvQueue;
	bool m_doDisconnect;

	std::string m_strUserName;
	std::string m_strPassword;
	std::string m_strUserNameInvalid;
	std::string m_strPasswordInvalid;
	std::string m_strAskInput;
	bool m_bLoginConfirm;

	EnumSpecialStep m_emStep;
	bool m_bLogin;
};

typedef std::shared_ptr<PuttyClient> PuttyClientPtr;

