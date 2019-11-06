#include "interaction.h"
#include <sstream>
#include <fstream>
#include "log/easylog.h"
#include "std/guid.h"
#include "putty_gen.h"
#include "live_client_handler.h"
#include "client_factory.h"
#include "ssh_client_param.h"

using namespace lw_util;
using namespace lw_client;

namespace lw_live {

	Interaction::Interaction()
		: m_clientPtr(nullptr)
		, m_displayPtr(nullptr)
		, m_destPtr(nullptr)
		, m_bLiveSucc(false)
	{
		m_filter.GetTerminal().Init(20, 240);
		m_offsetRow = 0;
	}

	Interaction::~Interaction()
	{
		WaitFree();

		SetClient(nullptr);
		m_lstPriKey.Clear();
	}

	DisplayPtr Interaction::GetDisplay()
	{
		return m_displayPtr;
	}

	void Interaction::SetDisplay(DisplayPtr displayPtr)
	{
		m_displayPtr = displayPtr;
	}

	vt_terminal* Interaction::GetTerminal()
	{
		return &m_terminal;
	}

	DestinationPtr Interaction::GetDestination()
	{
		return m_destPtr;
	}

	void Interaction::SetDestination(DestinationPtr dest)
	{
		m_destPtr = dest;
	}

	bool Interaction::IsLiveSucc()
	{
		return m_bLiveSucc;
	}

	void Interaction::SetLiveSucc(bool bSucc)
	{
		m_bLiveSucc = bSucc;
	}

	void Interaction::SetDataFolder(const std::string& strFolder)
	{
		m_strDataFolder = strFolder;
	}

	bool Interaction::IsConnected()
	{
		ClientPtr clientPtr = GetClient();
		return clientPtr->IsConnected();
	}

	bool Interaction::Connect(bool isRetry/* = false*/)
	{
		if (isRetry && IsLiveSucc())
		{
			return false;
		}

		SetLiveSucc(false);
		LiveParamPtr liveParam = nullptr;
		if (isRetry)
		{
			liveParam = m_destPtr->GetRetryClient(GetLiveParam());
		}
		else
		{
			liveParam = m_destPtr->GetClient();
		}

		while (liveParam)
		{
			if (ConnectClient(liveParam))
			{
				return true;
			}

			liveParam = m_destPtr->GetRetryClient(GetLiveParam());
		}

		return false;
	}

	bool Interaction::JumpClient(bool bChild)
	{
		if (!IsLiveSucc())
		{
			return false;
		}

		LiveParamPtr currLiveParam = GetLiveParam();
		LiveParamPtr liveParam = nullptr;
		if (bChild)
		{
			liveParam = m_destPtr->GetNextClient(currLiveParam);
		}
		else
		{
			liveParam = m_destPtr->GetRetryClient(currLiveParam);
		}

		return JumpClient(liveParam);
	}

	bool Interaction::DisConnect()
	{
		ClientPtr clientPtr = GetClient();
		if (nullptr == clientPtr)
		{
			EasyLog(InstLive, LOG_WARN, "the client is already closed.");
			return false;
		}

		return clientPtr->AsyncDisConnect();
	}

	void Interaction::SendData(const std::string& str, bool bComplete/* = true*/)
	{
		ClientPtr clientPtr = GetClient();
		if (nullptr == clientPtr)
		{
			EasyLog(InstLive, LOG_WARN, "are you sure the client exist?");
			return;
		}

		//you need add "\r" to confirm the input. such as input a enter
		if (bComplete)
		{
			std::stringstream ss;
			ss << str << "\r";

			clientPtr->Send(ss.str());
		}
		else
		{
			clientPtr->Send(str);
		}
	}
	
	void Interaction::WriteData(const char* str, int count)
	{
		m_terminal.parse(str, count);
		std::wstring wstr = m_terminal.get_all_text();

		m_filter.Parse(str, count);

		int nClipLines = 0;
		std::vector<VtLine> buffer;
		m_filter.GetClipLines(buffer, true);
		m_filter.ClearClipLines();

		nClipLines = buffer.size();

		m_filter.GetTerminal().GetBuffer(buffer);

		int row = 0, col = 0;
		m_filter.GetTerminal().GetCursorPos(col, row);

		DisplayPtr displayPtr = GetDisplay();
		if (displayPtr)
		{
			displayPtr->TrimLines(buffer, nClipLines + row, col);
			displayPtr->ReWriteLines(m_offsetRow, buffer);

			m_offsetRow += nClipLines;

			displayPtr->SeekP(m_offsetRow + row, col);
		}
	}

	void Interaction::SavePriKeyFile(ClientParam* pClientParam)
	{
		SshClientParam* pSshClientParam = dynamic_cast<SshClientParam*>(pClientParam);
		if (nullptr == pSshClientParam)
		{
			return;
		}

		std::string strTemp = pSshClientParam->GetPriKeyFile();
		if (!strTemp.empty())
		{
			return;
		}

		std::string strKeyName, strPriKey;
		pSshClientParam->GetPrivateKey(strKeyName, strPriKey);
		if (strPriKey.empty())
		{
			return;
		}

		GUID guid(true);
		std::string strFileName = guid.ToString();
		std::string strFilePath = m_strDataFolder + strFileName;

		{
			std::fstream file;
			file.open(strFilePath.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios::binary);
			file << strPriKey;
		}

		PuttyGen gen;
		if (!gen.IsPpkFile(strFilePath))
		{
			gen.SetPassphrase(pSshClientParam->GetPassword());

			std::string strErrMsg;
			if (!gen.LoadKeyFile(strFilePath, strErrMsg))
			{
				EasyLog(InstLive, LOG_ERROR, "ip:%s, failed to load private key file:%s, err:%s",
					pSshClientParam->GetHost().c_str(), strFilePath.c_str(), strErrMsg.c_str());
			}
			else
			{
				::remove(strFilePath.c_str());
				if (!gen.SavePpkFile(strFilePath, strErrMsg))
				{
					EasyLog(InstLive, LOG_ERROR, "ip:%s, failed to save private key file:%s, err:%s",
						pSshClientParam->GetHost().c_str(), strFilePath.c_str(), strErrMsg.c_str());
				}
			}
		}

		pSshClientParam->SetPriKeyFile(strFilePath);
		m_lstPriKey.Append(strFilePath);

		EasyLog(InstLive, LOG_INFO, "ip:%s, private key file:%s", pSshClientParam->GetHost().c_str(), strFilePath.c_str());
	}

	bool Interaction::CanAccept_unsafe(Interface* intf)
	{
		if (nullptr == intf)
		{
			return false;
		}

		ClientPtr clientPtr = GetClient();
		if (nullptr == clientPtr)
		{
			return false;
		}

		ClientHandlerPtr handler = clientPtr->GetHandler();
		if (nullptr == handler)
		{
			return false;
		}
		
		return handler.get() == intf->m_userdata;
	}

	ClientPtr Interaction::GetClient()
	{
		return m_clientPtr;
	}

	void Interaction::SetClient(ClientPtr client)
	{
		m_clientPre = m_clientPtr;
		m_clientPtr = client;
	}

	LiveParamPtr Interaction::GetLiveParam()
	{
		return m_liveParam;
	}

	void Interaction::SetLiveParam(LiveParamPtr liveParam)
	{
		m_liveParam = liveParam;
	}

	bool Interaction::ConnectClient(LiveParamPtr liveParam)
	{
		SetLiveParam(liveParam);
		if (nullptr == liveParam)
		{
			EasyLog(InstLive, LOG_ERROR, "error live param.");
			return false;
		}

		ClientParamPtr clientParam = liveParam->GetClientParam();
		if (nullptr == clientParam)
		{
			EasyLog(InstLive, LOG_ERROR, "error client param.");
			return false;
		}

		EasyLog(InstLive, LOG_INFO, "create client, host:%s, port:%d.", clientParam->GetHost().c_str(), clientParam->GetPort());
		ClientPtr clientPtr = ClientFactory::Create(clientParam.get());
		SetClient(clientPtr);

		if (nullptr == clientPtr)
		{
			EasyLog(InstLive, LOG_ERROR, "failed create client.");
			return false;
		}

		Startup();
		LiveClientHandlerPtr liveHandler = std::make_shared<LiveClientHandler>();
		liveHandler->SetInteraction(this);
		clientPtr->SetHandler(liveHandler);
		if (!clientPtr->Connect(clientParam.get()))
		{
			EasyLog(InstLive, LOG_ERROR, "failed to connect the destination.");
			return false;
		}

		SavePriKeyFile(clientParam.get());

		return true;
	}

	bool Interaction::JumpClient(LiveParamPtr liveParam)
	{
		SetLiveParam(liveParam);
		if (nullptr == liveParam)
		{
			EasyLog(InstLive, LOG_ERROR, "error live param.");
			return false;
		}

		ClientParamPtr clientParam = liveParam->GetClientParam();
		if (nullptr == clientParam)
		{
			EasyLog(InstLive, LOG_ERROR, "error client param.");
			return false;
		}
		std::string strConn = ClientFactory::GetConnectString(clientParam.get());
		if (strConn.empty())
		{
			EasyLog(InstLive, LOG_ERROR, "failed to get connection string.");
			return false;
		}

		SendData(strConn, true);

		SavePriKeyFile(clientParam.get());

		return true;
	}

}


