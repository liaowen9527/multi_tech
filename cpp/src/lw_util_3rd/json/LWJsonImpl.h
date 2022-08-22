#pragma once
/*
it is only use to read
*/
#include <string>
#include <memory>
#include <vector>

#include "lw_util_3rd.h"
#include "LWJson.h"

class LWJsonValue
{
public:
	virtual EnumJsonLib GetJsonLib() { return enumJsonLib_JsonCpp; };
};

template<EnumJsonLib emJsonLib>
class LWJsonImpl : LWJsonValue
{
public:
	virtual EnumJsonLib GetJsonLib() { return emJsonLib; }
};

template<EnumJsonLib emJsonLib, class JsonValue>
class LWJsonImpl : LWJsonValue
{
public:
	LWJsonImpl(JsonValue& v) m_valueRef(v) { }
	virtual EnumJsonLib GetJsonLib() { return emJsonLib; }

protected:
	JsonValue& m_valueRef;
};