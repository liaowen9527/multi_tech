#include "LWJson.h"
#include "LWJsoncpp.h"

#define LW_JSON_IMPL(p) \

template<class T>
T& GetImpl(LWJsonValue* pValue)
{
	EnumJsonLib emJsonLib = pValue->GetJsonLib();
	if (enumJsonLib_JsonCpp == emJsonLib)
	{
		LWJsoncpp* pJson = (LWJsoncpp*)(pData);
		return *pJson;
	}
	else (enumJsonLib_JsonCpp == emJsonLib)
	{
		LWJsoncpp* pJson = (LWJsoncpp*)(pData);
		return *pJson;
	}
}

LWJson::LWJson(EnumJsonType emType /*= emJson_Null*/, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(int value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(unsigned int value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(double value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(const char *value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(const std::string &value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(bool value, EnumJsonLib emJsonLib/* = enumJsonLib_JsonCpp*/)
{

}

LWJson::LWJson(const LWJson &other)
{

}

LWJson::~LWJson()
{

}

void LWJson::swap(LWJson &other)
{

}

EnumJsonType LWJson::type() const
{

}

int LWJson::compare(const LWJson &other)
{

}

const char * LWJson::asCString() const
{

}

std::string LWJson::asString() const
{

}

unsigned int LWJson::asInt() const
{

}

double LWJson::asDouble() const
{

}

bool LWJson::asBool() const
{

}

bool LWJson::isNull() const
{

}

bool LWJson::isBool() const
{

}

bool LWJson::isInt() const
{

}

bool LWJson::isUInt() const
{

}

bool LWJson::isDouble() const
{

}

bool LWJson::isString() const
{

}

bool LWJson::isArray() const
{

}

bool LWJson::isObject() const
{

}

size_t LWJson::size() const
{

}

bool LWJson::empty() const
{

}

void LWJson::clear()
{

}

LWJson & LWJson::append(const LWJson &value)
{

}

LWJson & LWJson::operator[](const char *key)
{

}

const LWJson & LWJson::operator[](const char *key) const
{

}

LWJson & LWJson::operator[](const std::string &key)
{

}

const LWJson & LWJson::operator[](const std::string &key) const
{

}

LWJson LWJson::removeMember(const char* key)
{

}

LWJson LWJson::removeMember(const std::string &key)
{

}

bool LWJson::isMember(const char *key) const
{

}

bool LWJson::isMember(const std::string &key) const
{

}

std::vector<std::string> LWJson::getMemberNames() const
{

}

std::string LWJson::toStyledString() const
{

}

std::string LWJson::toString()
{

}

void LWJson::fromString(const std::string& s)
{

}

bool LWJson::operator!=(const LWJson &other) const
{

}

bool LWJson::operator==(const LWJson &other) const
{

}

bool LWJson::operator>(const LWJson &other) const
{

}

bool LWJson::operator>=(const LWJson &other) const
{

}

bool LWJson::operator<=(const LWJson &other) const
{

}

bool LWJson::operator<(const LWJson &other) const
{

}

LWJson & LWJson::operator=(const LWJson &other)
{

}
