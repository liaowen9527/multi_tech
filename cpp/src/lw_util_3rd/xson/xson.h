#pragma once
/*
it is only use to read
*/
#include <string>
#include <memory>
#include <vector>

#include "lw_util_3rd.h"
#include "xson_element.h"

class xson_view;

class LWUtil3rd_API xson
{
public:
	xson(std::shared_ptr<void> data = nullptr);
	xson(xson_view& view);
	~xson();

public:
	void bind(void* data);
	void bind(std::shared_ptr<void> data);
	
	void* data() const;
	xson_view view() const;

	std::string to_string();

protected:
	//bsoncxx::document::value*
	std::shared_ptr<void> m_data;
};


class LWUtil3rd_API xson_view
{
public:
	xson_view(void* data = nullptr, int len = 0);
	~xson_view();

public:
	void* data() const;
	int length() const;

	std::string to_string() const;
 
	void get_elements(std::function<void(xson_element&)> parse_element) const;

protected:
	//bsoncxx::document::view.data()
	void* m_data;
	//bsoncxx::document::view.length()
	int m_length;
};