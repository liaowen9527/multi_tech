#include "status.h"
#include <time.h>

namespace lw_client {

	Status::Status()
		: m_status(DISCONNECTED)
	{

	}

	Status::~Status()
	{

	}

	bool Status::is_connecting()
	{
		return m_status == CONNECTING;
	}

	void Status::connecting()
	{
		m_status = CONNECTING;
	}

	bool Status::is_connected()
	{
		return m_status == CONNECTED;
	}

	void Status::connected()
	{
		m_status = CONNECTED;
	}

	bool Status::is_disconnecting()
	{
		return m_status == DISCONNECTING;
	}

	void Status::disconnecting()
	{
		m_status = DISCONNECTING;
	}

	bool Status::is_disconnected()
	{
		return m_status == DISCONNECTED;
	}

	void Status::disconnected()
	{
		m_status = DISCONNECTED;
	}

	bool Status::is_match(EnumStatus nStatus)
	{
		return m_status == nStatus;
	}


	AsyncStatus::AsyncStatus()
	{

	}

	AsyncStatus::~AsyncStatus()
	{

	}

	void AsyncStatus::connected()
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		Status::connected();
		m_condVar.notify_one();
	}

	bool AsyncStatus::wait_connected(int nTimeout/* = 60*/)
	{
		return wait_status(CONNECTED, nTimeout);
	}

	void AsyncStatus::disconnected()
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		Status::disconnected();
		m_condVar.notify_one();
	}

	bool AsyncStatus::wait_disconnected(int nTimeout/* = 60*/)
	{
		return wait_status(DISCONNECTED, nTimeout);
	}

	bool AsyncStatus::wait_status(EnumStatus nStatus, int nTimeout)
	{
		auto funcBreak = [this, nStatus]() {
			std::unique_lock<std::mutex> lck(m_mutex);
			if (this->m_status == nStatus)
			{
				return true;
			}

			return this->is_need_stop_wait_unsafe(nStatus, m_status);
		};

		return Wait(funcBreak, nTimeout);
	}

	bool AsyncStatus::is_need_stop_wait_unsafe(EnumStatus emExpect, EnumStatus emReal)
	{
		switch (emExpect)
		{
		case CONNECTED:
		{
			if (emReal != CONNECTING)
			{
				return true;
			}
			break;
		}
		case DISCONNECTED:
		{
			if (emReal != DISCONNECTING)
			{
				return true;
			}
			break;
		}
		default:
			break;
		}

		return false;
	}

}


