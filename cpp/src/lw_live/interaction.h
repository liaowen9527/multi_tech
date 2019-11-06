#pragma once
#include <atomic>
#include "lw_live.h"
#include "destination.h"
#include "lw_client.h"
#include "display.h"
#include "filesystem/filelist.h"
#include "filesystem/interface_svr.h"
#include "vtcharfilter.h"
#include "client.h"
#include "destination.h"
#include "vt100/vt_terminal.h"

namespace lw_live {

	class LWLive_API Interaction : public lw_util::InterfaceSvr
	{
	public:
		Interaction();
		~Interaction();

	public:
		DisplayPtr GetDisplay();
		void SetDisplay(DisplayPtr displayPtr);

		vt_terminal* GetTerminal();

		DestinationPtr GetDestination();
		void SetDestination(DestinationPtr dest);

		bool IsLiveSucc();
		void SetLiveSucc(bool bSucc);

		void SetDataFolder(const std::string& strFolder);

		void WriteData(const char* str, int count);

	public:
		bool IsConnected();
		bool Connect(bool isRetry = false);
		bool DisConnect();

		void SendData(const std::string& str, bool bComplete = true);

		bool JumpClient(bool bChild);

	public:
		void SavePriKeyFile(lw_client::ClientParam* pClientParam);

	protected:
		virtual bool CanAccept_unsafe(lw_util::Interface* intf);

	protected:
		lw_client::ClientPtr GetClient();
		void SetClient(lw_client::ClientPtr client);

		LiveParamPtr GetLiveParam();
		void SetLiveParam(LiveParamPtr liveParam);

		bool ConnectClient(LiveParamPtr liveParam);
		bool JumpClient(LiveParamPtr liveParam);

	protected:
		lw_client::ClientPtr m_clientPre;	//avoid hung when do the disconnect callback.
		lw_client::ClientPtr m_clientPtr;
		DisplayPtr m_displayPtr;

		DestinationPtr m_destPtr;
		LiveParamPtr m_liveParam;

		bool m_bLiveSucc;

	protected:
		CVtCharFilter m_filter;
		int m_offsetRow;

		vt_terminal m_terminal;

	protected:
		std::string m_strDataFolder;
		lw_util::FileList m_lstPriKey;
	};

	typedef std::shared_ptr<Interaction> InteractionPtr;
}


