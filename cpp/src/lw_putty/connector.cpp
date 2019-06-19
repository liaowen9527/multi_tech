#include "connector.h"
#include "putty_client.h"
#include "putty_impl.h"

Connector::Connector()
{
	m_bQuit = false;
	m_lastSocket = -1;
	m_lastNetEvent = 0;
	m_pThread = new std::thread(&Connector::DoConnect, this);
}

Connector::~Connector()
{
	m_bQuit = true;
	m_conditionVar.notify_one();

	if (m_pThread->joinable())
	{
		m_pThread->join();
	}
	delete m_pThread;
	m_pThread = nullptr;
}

Connector* Connector::Instance()
{
	static Connector* s_connector = nullptr;
	if (nullptr == s_connector)
	{
		s_connector = new Connector;
	}

	return s_connector;
}

void Connector::Init(PuttyClient* pClient, std::function<void()> func)
{
	ConnectTask task;
	task.pClient = pClient;
	task.callback = func;

	std::unique_lock<std::mutex> lck(m_mutex);
	m_queue.push(task);
	lck.unlock();

	m_conditionVar.notify_one();
}

WSAEVENT Connector::GetNetEvent(SOCKET skt, bool bNewIfNotExist/* = true*/)
{
	WSAEVENT netevent = 0;

	std::lock_guard<std::mutex> lck(m_mutex_event);
	auto itr = m_mapSocketEvent.find(skt);
	if (itr == m_mapSocketEvent.end())
	{
		netevent = CreateEvent(NULL, FALSE, FALSE, NULL);
		
		m_lastSocket = skt;
		m_lastNetEvent = netevent;
		m_mapSocketEvent.insert(std::make_pair(skt, netevent));
	}
	else
	{
		netevent = itr->second;
	}

	return netevent;
}

void Connector::CloseSocket(SOCKET skt)
{
	WSAEVENT netevent = GetNetEvent(skt, false);
	if (netevent)
	{
		std::lock_guard<std::mutex> lck(m_mutex_event);
		m_mapSocketEvent.erase(skt);
		CloseHandle(netevent);
	}
}

void Connector::DoConnect()
{
	do 
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		if (m_bQuit)
		{
			break;
		}
		if(m_queue.empty())
		{
			m_conditionVar.wait(lck);
		}

		if (!m_queue.empty())
		{
			ConnectTask task = m_queue.front();
			m_queue.pop();

			task.pClient->StartUpConn();
			task.pClient->SetWSAEvent(m_lastSocket, m_lastNetEvent);
			task.callback();
		}
		
	} while (true);
	
}
