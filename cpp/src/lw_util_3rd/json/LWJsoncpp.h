#pragma once
/*
it is only use to read
*/
#include <string>
#include <memory>
#include <vector>

#include "lw_util_3rd.h"
#include "LWJsonImpl.h"
#include <jsoncpp/json/value.h>

class LWJsoncpp : public LWJsonImpl<enumJsonLib_JsonCpp>
{
public:
	LWJsoncpp(EnumJsonType emType = emJson_Null);
	LWJsoncpp(int value);
	LWJsoncpp(unsigned int value);
	LWJsoncpp(double value);
	LWJsoncpp(const char *value);
	LWJsoncpp(const std::string &value);
	LWJsoncpp(bool value);
	LWJsoncpp(const LWJsoncpp &other);
	
	~LWJsoncpp();

public:
	void swap(LWJsoncpp &other);

	EnumJsonType type() const;

public:
	LWJsoncpp &operator=(const LWJsoncpp &other);

	bool operator <(const LWJsoncpp &other) const;
	bool operator <=(const LWJsoncpp &other) const;
	bool operator >=(const LWJsoncpp &other) const;
	bool operator >(const LWJsoncpp &other) const;

	bool operator ==(const LWJsoncpp &other) const;
	bool operator !=(const LWJsoncpp &other) const;

	int compare(const LWJsoncpp &other);

public:
	const char *asCString() const;
	std::string asString() const;

	unsigned int asInt() const;
	double asDouble() const;
	bool asBool() const;

	bool isNull() const;
	bool isBool() const;
	bool isInt() const;
	bool isUInt() const;
	bool isDouble() const;
	bool isString() const;
	bool isArray() const;
	bool isObject() const;

public:
	/// Number of values in array or object
	size_t size() const;

	/// \brief Return true if empty array, empty object, or null;
	/// otherwise, false.
	bool empty() const;

	/// Remove all object members and array elements.
	/// \pre type() is arrayValue, objectValue, or nullValue
	/// \post type() is unchanged
	void clear();

public:
	///
	/// Equivalent to jsonvalue[jsonvalue.size()] = value;
	LWJsoncpp &append(const LWJsoncpp &value);

	/// Access an object value by name, create a null member if it does not exist.
	LWJsoncpp &operator[](const char *key);
	/// Access an object value by name, returns null if there is no member with that name.
	const LWJsoncpp &operator[](const char *key) const;
	/// Access an object value by name, create a null member if it does not exist.
	LWJsoncpp &operator[](const std::string &key);
	/// Access an object value by name, returns null if there is no member with that name.
	const LWJsoncpp &operator[](const std::string &key) const;
	
	/// \brief Remove and return the named member.  
	///
	/// Do nothing if it did not exist.
	/// \return the removed Value, or null.
	/// \pre type() is objectValue or nullValue
	/// \post type() is unchanged
	LWJsoncpp removeMember(const char* key);
	/// Same as removeMember(const char*)
	LWJsoncpp removeMember(const std::string &key);

	/// Return true if the object has a member named key.
	bool isMember(const char *key) const;
	/// Return true if the object has a member named key.
	bool isMember(const std::string &key) const;

	/// \brief Return a list of the member names.
	///
	/// If null, return an empty list.
	/// \pre type() is objectValue or nullValue
	/// \post if type() was nullValue, it remains nullValue
	std::vector<std::string> getMemberNames() const;

	std::string toStyledString() const;

public:
	std::string toString();
	void fromString(const std::string& s);

protected:
	Json::Value m_value;
};
