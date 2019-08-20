#pragma once
#include <memory>
#include <string>
#include <functional>

#include "lw_util_3rd.h"

enum xson_value_type
{
	nullValue = 0,
	intValue,
	doubleValue,
	stringValue,
	booleanValue,
	arrayValue,
	documentValue,
	dateValue
};

class LWUtil3rd_API xson_element
{
public:
	xson_element(std::shared_ptr<void> element = nullptr);
	~xson_element();

	xson_value_type type() const;
	void* data() const;

	void output() const;

public:
	std::string get_key() const;

	bool is_null() const;

	bool is_bool() const;
	bool get_bool() const;

	bool is_int() const;
	int get_int() const;

	bool is_double() const;
	double get_double() const;

	bool is_string() const;
	std::string get_string() const;

	bool is_array() const;
	std::string get_array() const;
	void get_array(std::function<void(xson_element&)> parse_element) const;

	bool is_document() const;
	std::string get_document() const;

	bool is_date() const;
	time_t get_date() const;

protected:
	//bsoncxx::document::element*
	std::shared_ptr<void> m_element;
};