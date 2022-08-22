#pragma once
/*
it is only use to read
*/
#include <string>
#include <memory>
#include <vector>

#include "lw_util_3rd.h"

enum EnumJsonType
{
	emJson_Null = 0, ///< 'null' value
	emJson_Int,      ///< signed integer value
	emJson_UInt,     ///< unsigned integer value
	emJson_Double,     ///< double value
	emJson_String,   ///< UTF-8 string value
	emJson_Bool,  ///< bool value
	emJson_Array,    ///< array value (ordered list)
	emJson_Object    ///< object value (collection of name/value pairs).
};

enum EnumJsonLib 
{
	enumJsonLib_JsonCpp,
	EnumJsonLib_Bson
};

class LWJsonValue;

class LWUtil3rd_API LWJson
{
public:
	LWJson(EnumJsonType emType = emJson_Null, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(int value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(unsigned int value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(double value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(const char *value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(const std::string &value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(bool value, EnumJsonLib emJsonLib = enumJsonLib_JsonCpp);
	LWJson(const LWJson &other);
	
	~LWJson();

public:
	void swap(LWJson &other);

	EnumJsonType type() const;

public:
	LWJson &operator=(const LWJson &other);

	bool operator <(const LWJson &other) const;
	bool operator <=(const LWJson &other) const;
	bool operator >=(const LWJson &other) const;
	bool operator >(const LWJson &other) const;

	bool operator ==(const LWJson &other) const;
	bool operator !=(const LWJson &other) const;

	int compare(const LWJson &other);

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
	LWJson &append(const LWJson &value);

	/// Access an object value by name, create a null member if it does not exist.
	LWJson &operator[](const char *key);
	/// Access an object value by name, returns null if there is no member with that name.
	const LWJson &operator[](const char *key) const;
	/// Access an object value by name, create a null member if it does not exist.
	LWJson &operator[](const std::string &key);
	/// Access an object value by name, returns null if there is no member with that name.
	const LWJson &operator[](const std::string &key) const;
	
	/// \brief Remove and return the named member.  
	///
	/// Do nothing if it did not exist.
	/// \return the removed Value, or null.
	/// \pre type() is objectValue or nullValue
	/// \post type() is unchanged
	LWJson removeMember(const char* key);
	/// Same as removeMember(const char*)
	LWJson removeMember(const std::string &key);

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
	LWJsonValue m_value;
};
