#include "packet.h"
#include <assert.h>
#include "netkit.h"
#include "json/json.h"
#include "security/zipper.h"
#include "security/encrypter.h"
#include "log/easylog.h"

namespace netkit {

	PacketHead::PacketHead(int protocol/* = 0*/)
		: m_version(Version)
		, m_flag(0)
		, m_protocol(protocol)
	{
	}

	PacketHead::~PacketHead()
	{
	}

	const char* PacketHead::GetDelimeter()
	{
		return "\r\n\r\n";
	}

	const char* PacketHead::FindDelimeter(const char *data, size_t len)
	{
		auto delim = GetDelimeter();
		auto delimLen = strlen(delim);
		assert(delimLen == 4);	//4bit = sizeof(int)

		if (!data || len < delimLen) return nullptr;

		int delimBlock = *(int*)delim;
		int end = len - delimLen + 1;

		for (int i = 0; i < end; i++)
		{
			const char *dataCur = data + i;
			int dataBlock = *(int*)dataCur;
			if (dataBlock == delimBlock)
			{
				return dataCur;
			}
		}

		return nullptr;
	}

	bool PacketHead::Serialize(std::string& str) const
	{
		Json::Value root;
		root["version"] = head.GetVersion();
		root["flag"] = head.GetFlag();
		root["bodysize"] = GetBodySize();

		Json::FastWriter fast_writer;
		str = fast_writer.write(root);

		return true;
	}

	bool PacketHead::Deserialize(const std::string& str)
	{
		try
		{
			Json::Value root;
			if (!Json::Reader().parse(str, root))
			{
				EasyLog(InstNet, LOG_ERROR, "failed to format json, json:%s", str.c_str());
				return false;
			}

			m_version = root["version"].asInt();
			m_flag = root["flag"].asInt();
			m_bodysize = root["bodysize"].asUInt();

			return true;
		}
		catch (std::exception& e)
		{
			EasyLog(InstNet, LOG_ERROR, "exception:%s, json:%s", e.what(), str.c_str());
		}
		catch (...)
		{
			EasyLog(InstNet, LOG_ERROR, "unkown exception, json:%s", str.c_str());
		}

		return false;
	}

	bool PacketHead::CheckVersion() const
	{
		return GetVersion() == Version;
	}

	int PacketHead::GetVersion() const
	{
		return m_version;
	}

	void PacketHead::SetVersion(int version)
	{
		m_version = version;
	}

	int PacketHead::GetProtocol() const
	{
		return m_protocol;
	}

	void PacketHead::SetProtocol(int proto)
	{
		m_protocol = proto;
	}

	int PacketHead::GetFlag() const
	{
		return m_flag;
	}

	bool PacketHead::IsOrginBody() const
	{
		return m_flag == 0;
	}

	bool PacketHead::IsEncrypt() const
	{
		return m_flag&  0x01;
	}

	void PacketHead::SetEncrypt(bool value)
	{
		m_flag = m_flag | 0x01;
	}

	bool PacketHead::IsZipped() const
	{
		return m_flag&  0x11;
	}

	void PacketHead::SetZipped(bool value)
	{
		m_flag = m_flag | 0x11;
	}

	size_t PacketHead::GetBodySize() const
	{
		return m_bodysize;
	}

	Packet::Packet()
		: m_orgindata(true)
	{

	}

	Packet::~Packet()
	{

	}

	bool Packet::AllocBody()
	{
		auto bodySize = head.GetBodySize();

		try
		{
			m_body.resize(bodySize);
		}
		catch (std::exception& e)
		{
			EasyLog(InstNet, LOG_ERROR, "alloc packet body exctption, need %d, %s", bodySize, e.what());
			return false;
		}

		if (m_body.capacity() < bodySize)
		{
			EasyLog(InstNet, LOG_ERROR, "alloc packet body fail, need %d", bodySize);
			return false;
		}

		return true;
	}

	size_t Packet::GetBodySize() const
	{
		return m_body.size();
	}

	bool Packet::serialize(std::string& data, std::string& errMsg) const
	{
		Json::Value root;
		root["version"] = head.GetVersion();
		root["flag"] = head.GetFlag();
		root["bodysize"] = GetBodySize();

		if (head.IsOrginBody())
		{
			root["body"] = temp;
			return true;
		}

		std::string temp;
		if (head.IsEncrypt())
		{
			Encrypter encrypt;
			encrypt.Encrypt(data, temp);
		}
		if (head.IsZipped())
		{
			Zipper zip;
			zip.Zip(data, temp);
		}

		root["body"] = temp;

		Json::FastWriter fast_writer;
		data = fast_writer.write(root);

		return true;
	}

	bool Packet::Serialize(std::string& data) const
	{
		std::string strHeader;
		if (!m_head.Serialize(strHeader))
		{
			return false;
		}
		
		std::string strBody;
		SerializeBody(strBody);
	}

	bool Packet::Deserialize(const std::string& data)
	{

	}

	bool Packet::SerializeBody(std::string& data) const
	{
		if (head.IsOrginBody())
		{
			data = m_body;
			return true;
		}

		std::string temp;
		if (m_head.IsEncrypt())
		{
			Encrypter encrypt;
			if (!encrypt.Encrypt(m_body, temp))
			{
				return false;
			}
		}
		
		if (m_head.IsZipped())
		{
			std::string temp2;
			Zipper zip;
			zip.Zip(data, temp);
		}

		data.swap(temp);
	}

	bool Packet::DeserializeBody(std::string& data) const
	{

	}

}