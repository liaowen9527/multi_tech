
#pragma once

#include "lw_util.h"
#include <string>

namespace lw_util {

	class LWUtil_API GUID
	{
	public:
		GUID(bool bGenerate = false);
		GUID(const char* strGuid);
		~GUID();

		GUID& operator =(const GUID & id);
		bool operator ==(const GUID & id) const;
		bool operator !=(const GUID & id) const;

		bool IsValid() const;

		void SetNull();
		void GenerateGuid();

		std::string ToString() const;
		bool FromString(const char* strGuid);

	protected:
		void* m_data;
	};

}