#include "notify.h"

namespace lw_client {

	void Notify::OnConnected()
	{

	}

	void Notify::OnDisconnected(int nErrCode, const std::string& err_msg)
	{

	}

	void Notify::OnReceived(const char* str, int nLen)
	{

	}

	void Notify::OnError(int nErrCode, const std::string& err_msg)
	{

	}

	void Notify::OnLoginResult(bool bValue)
	{

	}


	NotifyEx::NotifyEx()
		: m_pNotify(nullptr)
		, m_bActive(false)
	{

	}

	NotifyEx::~NotifyEx()
	{

	}

	void NotifyEx::SetNotify(Notify* pNotify)
	{
		std::lock_guard<std::mutex> lck(m_mutex);

		while (m_bActive)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		m_pNotify = pNotify;
	}

	void NotifyEx::SetActive_unsafe(bool bActive)
	{
		m_bActive = bActive;
	}

	void NotifyEx::OnConnected()
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		SetActive_unsafe(true);

		if (m_pNotify)
		{
			m_pNotify->OnConnected();
		}

		SetActive_unsafe(false);
	}

	void NotifyEx::OnDisconnected(int nErrCode, const std::string& err_msg)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		SetActive_unsafe(true);

		if (m_pNotify)
		{
			m_pNotify->OnDisconnected(nErrCode, err_msg);
		}

		SetActive_unsafe(false);
	}

	void NotifyEx::OnReceived(const char* str, int nLen)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		SetActive_unsafe(true);

		if (m_pNotify)
		{
			m_pNotify->OnReceived(str, nLen);
		}

		SetActive_unsafe(false);
	}

	void NotifyEx::OnError(int nErrCode, const std::string& err_msg)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		SetActive_unsafe(true);

		if (m_pNotify)
		{
			m_pNotify->OnError(nErrCode, err_msg);
		}

		SetActive_unsafe(false);
	}

	void NotifyEx::OnLoginResult(bool bValue)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		SetActive_unsafe(true);

		if (m_pNotify)
		{
			m_pNotify->OnLoginResult(bValue);
		}

		SetActive_unsafe(false);
	}

}