#pragma once
#include <string>
#include <vector>
#include "lw_util.h"
#include <atomic>
#include <memory>


namespace netkit {

#ifndef NetkitApi
#define NetkitApi LWUtil_API
#endif

	class NetkitApi PacketHead
	{
	public:
		PacketHead(int protocol = 0);
		~PacketHead();

		static const char* GetDelimeter();
		static const char* FindDelimeter(const char *data, size_t len);

		bool Serialize(std::string& str) const;
		bool Deserialize(const std::string& str);

		bool CheckVersion() const;
		int GetVersion() const;
		void SetVersion(int version);

		int GetProtocol() const;
		void SetProtocol(int proto);

		int GetFlag() const;

		bool IsOrginBody() const;

		bool IsEncrypt() const;
		void SetEncrypt(bool value);

		bool IsZipped() const;
		void SetZipped(bool value);

		size_t GetBodySize() const;

		enum { Version = 1 };

	protected:
		int m_version;
		int m_flag;
		int m_protocol;
		size_t m_bodysize;
	};

	class NetkitApi Packet
	{
	public:
		Packet();
		~Packet();

		bool AllocBody();
		size_t GetBodySize() const;

		bool Serialize(std::string& data) const;
		bool Deserialize(const std::string& data);

		bool SerializeBody(std::string& data) const;
		bool DeserializeBody(std::string& data) const;

	public:
		PacketHead m_head;
		std::string m_body;
		bool m_orgindata;
	};

}
