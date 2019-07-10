#pragma once

#include <atomic>
#include "wait_event.h"

namespace lw_client {

	class Status
	{
	public:
		enum EnumStatus
		{
			CONNECTING = 1,
			CONNECTED,
			DISCONNECTING,
			DISCONNECTED
		};

	public:
		Status();
		virtual ~Status();

	public:
		bool is_connecting();
		void connecting();

		bool is_connected();
		virtual void connected();

		bool is_disconnecting();
		void disconnecting();

		bool is_disconnected();
		virtual void disconnected();

	protected:
		bool is_match(EnumStatus nStatus);

	protected:
		EnumStatus m_status;
	};


	class AsyncStatus : public Status, public WaitEvent
	{
	public:
		AsyncStatus();
		~AsyncStatus();

	public:
		void connected();
		//timeout unit: second
		bool wait_connected(int nTimeout = 60);

		void disconnected();
		//timeout unit: second
		bool wait_disconnected(int nTimeout = 60);

	protected:
		bool wait_status(EnumStatus nStatus, int nTimeout);
		bool is_need_stop_wait_unsafe(EnumStatus nExpectStatus, EnumStatus nRealStatus);

	protected:
		std::mutex m_mutex;
	};

}

