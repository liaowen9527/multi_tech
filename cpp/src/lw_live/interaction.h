#pragma once
#include <atomic>
#include "lw_live.h"
#include "destination.h"
#include "lw_client.h"
#include "display.h"
#include "filelist.h"
#include "filesystem/interface_svr.h"
#include "vtcharfilter.h"

namespace lw_live {

	class LWLive_API Interaction : lw_util::InterfaceSvr
	{
	public:
		Interaction();
		~Interaction();

	public:
		DisplayPtr GetDisplay();
		void SetDisplay(DisplayPtr displayPtr);

		DestinationPtr GetDestination();
		void SetDestination(DestinationPtr destPtr);

		void SetDataFolder(const std::string& strFolder);

		void WriteData(const char* str, int count);

	public:
		bool IsConnected();
		bool Connect();
		bool DisConnect();

		void SendData(const std::string& str, bool bComplete = true);

	public:
		void SavePriKeyFile(ClientParam* pClientParam);

	protected:
		virtual bool CanAccept_unsafe(Interface* intf);

	protected:
		ClientPtr GetClient();
		void SetClient(ClientPtr client);

	protected:
		ClientPtr m_clientPtr;
		DestinationPtr m_destPtr;
		DisplayPtr m_displayPtr;

	protected:
		CVtCharFilter m_filter;
		int m_offsetRow;

	protected:
		std::string m_strDataFolder;
		FileList m_lstPriKey;
	};

	typedef std::shared_ptr<Interaction> InteractionPtr;

	class LiveInterface : public lw_util::Interface
	{
	public:
		ClientHandler* m_handler;
	};
	

}


