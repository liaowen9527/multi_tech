#include "interface_svr.h"
#include "std/find_safe_ptr.h"

namespace lw_util {

	InterfaceSvr::InterfaceSvr(int maxIntf/* = 1*/)
		: m_nMaxIntf(maxIntf)
	{
		
	}

	InterfaceSvr::~InterfaceSvr()
	{

	}

	InterfaceSvrPtr InterfaceSvr::GetSafeSvr(InterfaceSvr* pSvr)
	{
		return FindSafePtr<InterfaceSvr>::Find(pSvr);
	}

	void InterfaceSvr::Startup()
	{
		FindSafePtr<InterfaceSvr>::Register(this->shared_from_this());
	}

	void InterfaceSvr::Shutdown()
	{
		FindSafePtr<InterfaceSvr>::UnRegister(this);
	}

	bool InterfaceSvr::Accept(Interface* intf, int timeout/* = -1*/)
	{
		time_t tm1 = time(0);
		while (true)
		{
			std::unique_lock<std::mutex> lck(m_mutexIntf);

			EnumAcceptResult result = TryAccept_unsafe(intf);
			if (emReject == result)
			{
				return false;
			}
			if (emSucc == result)
			{
				return true;
			}

			if (timeout < 0)
			{
				m_condVarIntf.wait(lck);
			}
			else
			{
				time_t tm2 = time(0);
				if (tm2 - tm1 > timeout)
				{
					return false;
				}

				m_condVarIntf.wait_for(lck, std::chrono::milliseconds(100));
			}
		}

		return false;
	}

	void InterfaceSvr::Close(Interface* intf)
	{
		std::unique_lock<std::mutex> lck(m_mutexIntf);
		m_intfs.erase(intf);

		m_condVarIntf.notify_one();
	}

	void InterfaceSvr::WaitFree()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lck(m_mutexIntf);
			if (m_intfs.empty())
			{
				break;
			}

			m_condVarIntf.wait_for(lck, std::chrono::milliseconds(100));
		}
	}

	EnumAcceptResult InterfaceSvr::TryAccept_unsafe(Interface* intf)
	{
		if (!CanAccept_unsafe(intf))
		{
			return emReject;
		}

		if (m_intfs.size() < m_nMaxIntf)
		{
			m_intfs.insert(intf);
			return emSucc;
		}

		return emFail;
	}

	bool InterfaceSvr::CanAccept_unsafe(Interface* intf)
	{
		return true;
	}


	Interface::Interface()
		: m_svr(nullptr)
		, m_userdata(nullptr)
	{
		
	}

	Interface::~Interface()
	{
		Close();
	}

	bool Interface::Open(InterfaceSvrPtr svr)
	{
		Close();

		m_svr = svr;
		if (m_svr)
		{
			return m_svr->Accept(this);
		}
		return false;
	}

	void Interface::Close()
	{
		if (m_svr)
		{
			m_svr->Close(this);
		}
		m_svr = nullptr;
	}

}
