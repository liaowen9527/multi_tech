#include "guid.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace lw_util {

	GUID::GUID(bool bGenerate /*= false*/)
	{
		m_data = nullptr;
		if (bGenerate)
		{
			GenerateGuid();
		}
	}

	GUID::GUID(const char* strGuid)
	{

	}

	GUID::~GUID()
	{
		delete m_data;
		m_data = nullptr;
	}

	GUID& GUID::operator=(const GUID & id)
	{
		if (*this != id)
		{
			SetNull();
			boost::uuids::uuid* pUUID2 = (boost::uuids::uuid*)id.m_data;
			boost::uuids::uuid* pUUID = new boost::uuids::uuid;
			*pUUID = *pUUID2;
		}

		return *this;
	}

	bool GUID::operator!=(const GUID & id) const
	{
		boost::uuids::uuid* pUUID = (boost::uuids::uuid*)m_data;
		boost::uuids::uuid* pUUID2 = (boost::uuids::uuid*)id.m_data;
		return *pUUID != *pUUID2;
	}

	bool GUID::operator==(const GUID & id) const
	{
		boost::uuids::uuid* pUUID = (boost::uuids::uuid*)m_data;
		boost::uuids::uuid* pUUID2 = (boost::uuids::uuid*)id.m_data;
		return *pUUID == *pUUID2;
	}

	bool GUID::IsValid() const
	{
		if (nullptr == m_data)
		{
			return false;
		}

		boost::uuids::uuid* pUUID = (boost::uuids::uuid*)m_data;
		return !pUUID->is_nil();
	}

	void GUID::SetNull()
	{
		if (m_data)
		{
			delete m_data;
			m_data = nullptr;
		}
	}

	void GUID::GenerateGuid()
	{
		SetNull();

		boost::uuids::uuid* pUUID = new boost::uuids::uuid;
		boost::uuids::random_generator gen;
		*pUUID = gen();

		m_data = pUUID;
	}

	std::string GUID::ToString() const
	{
		std::string rtn;
		if (!IsValid())
		{
			boost::uuids::uuid uid = boost::uuids::nil_uuid();
			rtn = boost::uuids::to_string(uid);
		}
		else
		{
			boost::uuids::uuid* pUUID = (boost::uuids::uuid*)m_data;
			rtn = boost::uuids::to_string(*pUUID);
		}

		return rtn;
	}

	bool GUID::FromString(const char* strGuid)
	{
		SetNull();

		boost::uuids::uuid* pUUID = new boost::uuids::uuid;
		boost::uuids::string_generator gen;
		*pUUID = gen(strGuid);
		if (pUUID->is_nil())
		{
			return false;
		}

		m_data = pUUID;

		return true;
	}

}
