#pragma once
#include <string>
#include <memory>
#include <mutex>
#include "lw_client.h"

namespace lw_client {

	class LWClient_API Notify
	{
	public:
		virtual void OnConnected();;
		virtual void OnDisconnected(int nErrCode, const std::string& err_msg);;

		virtual void OnReceived(const char* str, int nLen);;

		virtual void OnError(int nErrCode, const std::string& err_msg);;

		virtual void OnLoginResult(bool bValue);
	};

	class LWClient_API NotifyEx
	{
	public:
		NotifyEx();
		~NotifyEx();

		void SetNotify(Notify* pNotify);

	public:
		void OnConnected();;
		void OnDisconnected(int nErrCode, const std::string& err_msg);;

		void OnReceived(const char* str, int nLen);;

		void OnError(int nErrCode, const std::string& err_msg);;

		void OnLoginResult(bool bValue);

	protected:
		void SetActive_unsafe(bool bActive);

	protected:
		std::mutex m_mutex;
		bool m_bActive;
		Notify* m_pNotify;
	};

}

