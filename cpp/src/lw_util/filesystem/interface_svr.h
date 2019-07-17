
#pragma once

#include "lw_util.h"
#include <string>
#include <set>
#include <memory>
#include <mutex>

namespace lw_util {

	enum EnumAcceptResult
	{
		emReject = 0,
		emSucc,
		emFail
	};

	class Interface;

	class LWUtil_API InterfaceSvr
	{
	public:
		InterfaceSvr(int maxIntf = 1);
		virtual ~InterfaceSvr();

	public:
		bool Accept(Interface* intf, int timeout = -1);
		void Close(Interface* intf);

		void WaitFree();

	protected:
		EnumAcceptResult TryAccept_unsafe(Interface* intf);
		virtual bool CanAccept_unsafe(Interface* intf);

	protected:
		std::condition_variable m_condVarIntf;
		std::mutex m_mutexIntf;

		std::set<Interface*> m_intfs;
		int m_nMaxIntf;
	};
	typedef std::shared_ptr<InterfaceSvr> InterfaceSvrPtr;


	class Interface
	{
	public:
		Interface(InterfaceSvrPtr svr);
		virtual ~Interface();

	public:
		InterfaceSvrPtr m_svr;
	};

}