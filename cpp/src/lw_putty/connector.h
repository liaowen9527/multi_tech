#pragma once
/*
* brief: for bind WSASockect to the client
* author: LiaoWen
*/
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <WinSock2.h>

class PuttyClient;

struct ConnectTask
{
	PuttyClient* pClient;
	std::function<void()> callback;
};

class Connector
{
public:
	Connector();
	~Connector();

	static Connector* Instance();

public:
	void Init(PuttyClient* pClient, std::function<void()> func);

	WSAEVENT GetNetEvent(SOCKET skt, bool bNewIfNotExist = true);
	void CloseSocket(SOCKET skt);

protected:
	void DoConnect();

protected:
	std::mutex m_mutex;
	std::mutex m_mutex_event;
	std::condition_variable m_conditionVar;

	std::thread* m_pThread;

	std::queue<ConnectTask> m_queue;
	std::map<SOCKET, WSAEVENT> m_mapSocketEvent;

	SOCKET m_lastSocket;
	WSAEVENT m_lastNetEvent;

	bool m_bQuit;
};