#include "xson_element.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <bsoncxx/json.hpp>
#include "xson_bson.h"

xson_element::xson_element(std::shared_ptr<void> element/* = nullptr*/)
	: m_element(element)
{
	
}

xson_element::~xson_element()
{

}

xson_value_type xson_element::type() const
{
	if (nullptr == m_element)
	{
		return nullValue;
	}
	
	xson_value_type xsontype = nullValue;

	/*k_array; k_binary; k_bool; k_code; k_codewscope; k_date; k_dbpointer;
	k_decimal128;  k_document; k_double; k_int32; k_int64; k_maxkey; k_minkey;
	k_null; k_oid; k_regex; k_symbol; k_timestamp; k_undefined; k_utf8;*/

	bsoncxx::type bsontype = to_element(*this).type();
	switch (bsontype)
	{
	case bsoncxx::type::k_null:
		xsontype = nullValue;
		break;
	case bsoncxx::type::k_int32:
	case bsoncxx::type::k_int64:
		xsontype = intValue;
		break;
	case bsoncxx::type::k_double:
		xsontype = doubleValue;
		break;
	case bsoncxx::type::k_utf8:
	case bsoncxx::type::k_oid:
	case bsoncxx::type::k_timestamp:
		xsontype = stringValue;
		break;
	case bsoncxx::type::k_bool:
		xsontype = booleanValue;
		break;
	case bsoncxx::type::k_array:
		xsontype = arrayValue;
		break;
	case bsoncxx::type::k_document:
		xsontype = documentValue;
		break;
	case bsoncxx::type::k_date:
		xsontype = dateValue;
		break;
	default:
		assert(false);
		break;
	}

	return xsontype;
}

void* xson_element::data() const
{
	return m_element.get();
}

void xson_element::output() const
{
	std::cout << "key=" << get_key() << "; ";

	switch (type())
	{
	case nullValue:
		std::cout << "null" << std::endl;
		break;
	case intValue:
		std::cout << "int = " << get_int() << std::endl;
		break;
	case doubleValue:
		std::cout << "double = " << get_double() << std::endl;
		break;
	case stringValue:
		std::cout << "string = " << get_string() << std::endl;
		break;
	case booleanValue:
		std::cout << "bool = " << get_bool() << std::endl;
		break;
	case arrayValue:
		std::cout << "array = " << get_array() << std::endl;
		break;
	case documentValue:
		std::cout << "document = " << get_document() << std::endl;
		break;
	case dateValue:
		std::cout << "date = " << get_date() << std::endl;
		break;
	default:
		assert(false);
		break;
	}
}

std::string xson_element::get_key() const
{
	return to_element(*this).key().to_string();
}

bool xson_element::is_null() const
{
	return nullValue == type();
}

bool xson_element::is_bool() const
{
	return booleanValue == type();
}

bool xson_element::get_bool() const
{
	return to_element(*this).get_bool();
}

bool xson_element::is_int() const
{
	return intValue == type();
}

int xson_element::get_int() const
{
	return to_element(*this).get_int32();
}

bool xson_element::is_double() const
{
	return doubleValue == type();
}

double xson_element::get_double() const
{
	return to_element(*this).get_double();
}

bool xson_element::is_string() const
{
	return stringValue == type();
}

std::string xson_element::get_string() const
{
	bsoncxx::document::element ele = to_element(*this);
	bsoncxx::type bsontype = ele.type();
	switch (bsontype)
	{
	case bsoncxx::type::k_utf8:
		return ele.get_utf8().value.to_string();
	case bsoncxx::type::k_oid:
		return ele.get_oid().value.to_string();
	case bsoncxx::type::k_timestamp:
	{
		std::stringstream ss;
		ss << "{t:" << ele.get_timestamp().timestamp << ", i:" << ele.get_timestamp().increment << "}";
		return ss.str();
	}	
	default:
		assert(false);
		break;
	}

	return "";
}

bool xson_element::is_array() const
{
	return arrayValue == type();
}

std::string xson_element::get_array() const
{
	bsoncxx::array::view v(to_element(*this).get_array().value);
	
	return bsoncxx::to_json(v);
}

void xson_element::get_array(std::function<void(xson_element&)> parse_element) const
{
	bsoncxx::array::view v(to_element(*this).get_array().value);

	for (auto&& ele : v)
	{
		parse_element(to_element(ele));
	}
}

bool xson_element::is_document() const
{
	return documentValue == type();
}

std::string xson_element::get_document() const
{
	bsoncxx::document::view v(to_element(*this).get_document().value);

	return bsoncxx::to_json(v);
}

bool xson_element::is_date() const
{
	return dateValue == type();
}

time_t xson_element::get_date() const
{
	return to_element(*this).get_date();
}

