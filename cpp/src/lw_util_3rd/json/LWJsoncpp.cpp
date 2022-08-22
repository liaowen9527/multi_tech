#include "LWJsoncpp.h"
#include <assert.h>

inline EnumJsonType FromJsoncppType(Json::ValueType emType)
{
	switch (emType)
	{
	case Json::nullValue:
		return emJson_Null;
	case Json::intValue:
		return emJson_Int;
	case Json::uintValue:
		return emJson_UInt;
	case Json::realValue:
		return emJson_Double;
	case Json::stringValue:
		return emJson_String;
	case Json::booleanValue:
		return emJson_Bool;
	case Json::arrayValue:
		return emJson_Array;
	case Json::objectValue:
		return emJson_Object;
	default:
		assert(false);
		return emJson_Null;
	}
}

inline Json::ValueType ToJsoncppType(EnumJsonType emType)
{
	switch (emType)
	{
	case emJson_Null:
		return Json::nullValue;
	case emJson_Int:
		return Json::intValue;
	case emJson_UInt:
		return Json::uintValue;
	case emJson_Double:
		return Json::realValue;
	case emJson_String:
		return Json::stringValue;
	case emJson_Bool:
		return Json::booleanValue;
	case emJson_Array:
		return Json::arrayValue;
	case emJson_Object:
		return Json::objectValue;
	default:
		assert(false);
		return Json::nullValue;
	}
}

LWJsoncpp::LWJsoncpp(EnumJsonType emType /*= emJson_Null*/)
	: m_value(ToJsoncppType(emType))
{

}

LWJsoncpp::LWJsoncpp(int value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(unsigned int value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(double value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(const char *value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(const std::string &value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(bool value)
	: m_value(value)
{

}

LWJsoncpp::LWJsoncpp(const LWJsoncpp &other)
	: m_value(other.m_value)
{

}

LWJsoncpp::~LWJsoncpp()
{

}

void LWJsoncpp::swap(LWJsoncpp &other)
{
	m_value.swap(other.m_value);
}

EnumJsonType LWJsoncpp::type() const
{
	m_value.type();
}

int LWJsoncpp::compare(const LWJsoncpp &other)
{
	return m_value.compare(other.m_value);
}

const char * LWJsoncpp::asCString() const
{
	return m_value.asCString();
}

std::string LWJsoncpp::asString() const
{
	return m_value.asString();
}

unsigned int LWJsoncpp::asInt() const
{
	return m_value.asInt();
}

double LWJsoncpp::asDouble() const
{
	return m_value.asDouble();
}

bool LWJsoncpp::asBool() const
{
	return m_value.asBool();
}

bool LWJsoncpp::isNull() const
{
	return m_value.isNull();
}

bool LWJsoncpp::isBool() const
{
	return m_value.isBool();
}

bool LWJsoncpp::isInt() const
{
	return m_value.isInt();
}

bool LWJsoncpp::isUInt() const
{
	return m_value.isUInt();
}

bool LWJsoncpp::isDouble() const
{
	return m_value.isDouble();
}

bool LWJsoncpp::isString() const
{
	return m_value.isString();
}

bool LWJsoncpp::isArray() const
{
	return m_value.isArray();
}

bool LWJsoncpp::isObject() const
{
	return m_value.isObject();
}

size_t LWJsoncpp::size() const
{
	return m_value.size();
}

bool LWJsoncpp::empty() const
{
	return m_value.empty();
}

void LWJsoncpp::clear()
{
	m_value.clear();
}

LWJsoncpp & LWJsoncpp::append(const LWJsoncpp &value)
{
	Json::Value& temp = m_value.append(value.m_value);

}

LWJsoncpp & LWJsoncpp::operator[](const char *key)
{
	Json::Value& temp = m_value[key];
}

const LWJsoncpp & LWJsoncpp::operator[](const char *key) const
{

}

LWJsoncpp & LWJsoncpp::operator[](const std::string &key)
{

}

const LWJsoncpp & LWJsoncpp::operator[](const std::string &key) const
{

}

LWJsoncpp LWJsoncpp::removeMember(const char* key)
{

}

LWJsoncpp LWJsoncpp::removeMember(const std::string &key)
{
	m_value.removeMember(key);
}

bool LWJsoncpp::isMember(const char *key) const
{
	return m_value.isMember(key);
}

bool LWJsoncpp::isMember(const std::string &key) const
{
	return m_value.isMember(key);
}

std::vector<std::string> LWJsoncpp::getMemberNames() const
{
	return m_value.getMemberNames();
}

std::string LWJsoncpp::toStyledString() const
{

}

std::string LWJsoncpp::toString()
{

}

void LWJsoncpp::fromString(const std::string& s)
{

}

bool LWJsoncpp::operator!=(const LWJsoncpp &other) const
{

}

bool LWJsoncpp::operator==(const LWJsoncpp &other) const
{

}

bool LWJsoncpp::operator>(const LWJsoncpp &other) const
{

}

bool LWJsoncpp::operator>=(const LWJsoncpp &other) const
{

}

bool LWJsoncpp::operator<=(const LWJsoncpp &other) const
{

}

bool LWJsoncpp::operator<(const LWJsoncpp &other) const
{

}

LWJsoncpp & LWJsoncpp::operator=(const LWJsoncpp &other)
{

}
