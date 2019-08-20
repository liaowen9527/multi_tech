#include "xson.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include "xson_bson.h"

//static bsoncxx::builder::base::document s_document;
static bsoncxx::document::value s_doc = bsoncxx::builder::stream::document().extract();

xson::xson(std::shared_ptr<void> data /*= nullptr*/)
	: m_data(data)
{

}

xson::xson(xson_view& view)
{
	bsoncxx::document::value* pdoc = new bsoncxx::document::value(to_bsonview(view));
	bind(pdoc);
}

xson::~xson()
{

}

void xson::bind(void* data)
{
	bind(std::shared_ptr<void>(data));
}

void xson::bind(std::shared_ptr<void> data)
{
	m_data = data;
}

void* xson::data() const
{
	if (m_data)
	{
		return m_data.get();
	}

	return &s_doc;
}

xson_view xson::view() const
{
	bsoncxx::document::view v = to_bsonview(*this);

	return to_xsonview(v);
}

std::string xson::to_string()
{
	return bsoncxx::to_json(to_bsonview(*this));
}

xson_view::xson_view(void* data /*= nullptr*/, int len/* = 0*/)
	: m_data(data)
	, m_length(len)
{

}

xson_view::~xson_view()
{

}

void* xson_view::data() const
{
	return m_data;
}

int xson_view::length() const
{
	return m_length;
}

std::string xson_view::to_string() const
{
	return bsoncxx::to_json(to_bsonview(*this));
}

void xson_view::get_elements(std::function<void(xson_element&)> parse_element) const
{
	bsoncxx::document::view v = to_bsonview(*this);
	for (auto&& ele : v)
	{
		parse_element(to_element(ele));
	}
}
