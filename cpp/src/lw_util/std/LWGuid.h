
#pragma once

#include "lw_util.h"
#include <string>

class LWUtil_API LWGuid
{
public:
	LWGuid(bool bGenerate = false);
	LWGuid(const char* strGuid);
	~LWGuid();

	LWGuid& operator =(const LWGuid & id);
	bool operator ==(const LWGuid & id) const;
	bool operator !=(const LWGuid & id) const;

	bool IsValid() const;

	void SetNull();
	void GenerateGuid();

	std::string ToString() const;
	bool FromString(const char* strGuid);

protected:
	void* m_data;
};