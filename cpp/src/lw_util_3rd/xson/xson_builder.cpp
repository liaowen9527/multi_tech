#include "xson_builder.h"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include "xson_bson.h"

#define to_document(p) ((bsoncxx::builder::basic::document*)(p))
#define to_subdocument(p) ((bsoncxx::builder::basic::sub_document*)(p))
#define to_array(p) ((bsoncxx::builder::basic::array*)(p))
#define to_subarray(p) ((bsoncxx::builder::basic::sub_array*)(p))


xson_builder::xson_builder()
{
	m_bSub = false;
	m_bArray = false;

	m_builder = new bsoncxx::builder::basic::document;
}

xson_builder::xson_builder(bool bSub, bool bArray, void* builder)
{
	m_bSub = bSub;
	m_bArray = bArray;
	m_builder = builder;
}

xson_builder::~xson_builder()
{
	if (!m_bSub)
	{
		delete m_builder;
		m_builder = nullptr;
	}
}

xson xson_builder::extract()
{
	assert(!m_bSub && !m_bArray);

	return to_xson(to_document(m_builder)->extract());
}

xson_view xson_builder::view()
{
	assert(!m_bSub && !m_bArray);

	return to_xsonview(to_document(m_builder)->view());
}

void xson_builder::clear()
{
	assert(!m_bSub);

	to_document(m_builder)->clear();
}

void xson_builder::append(int val)
{
	auto builder = to_subarray(m_builder);
	builder->append(val);
}

void xson_builder::append(const std::string& key, int val)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, val));
}

void xson_builder::append(double val)
{
	auto builder = to_subarray(m_builder);
	builder->append(val);
}

void xson_builder::append(const std::string& key, double val)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, val));
}

void xson_builder::append(const char* str)
{
	auto builder = to_subarray(m_builder);
	builder->append(str);
}

void xson_builder::append(const std::string& key, const char* str)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, str));
}

void xson_builder::append(const std::string& val)
{
	auto builder = to_subarray(m_builder);
	builder->append(val);
}

void xson_builder::append(const std::string& key, const std::string& val)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, val));
}

void xson_builder::append(bool val)
{
	auto builder = to_subarray(m_builder);
	builder->append(val);
}

void xson_builder::append(const std::string& key, bool val)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, val));
}

void xson_builder::append(time_t msec)
{
	std::chrono::milliseconds temp(msec);
	auto builder = to_subarray(m_builder);
	builder->append(bsoncxx::types::b_date(temp));
}

void xson_builder::append(const std::string& key, time_t msec)
{
	std::chrono::milliseconds temp(msec);
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, bsoncxx::types::b_date(temp)));
}

void xson_builder::append(const xson& doc)
{
	auto builder = to_subarray(m_builder);
	builder->append(to_bsonview(doc));
}

void xson_builder::append(const std::string& key, const xson& doc)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, to_bsonview(doc)));
}

void xson_builder::append(const xson_view& view)
{
	auto builder = to_subarray(m_builder);
	builder->append(to_bsonview(view));
}

void xson_builder::append(const std::string& key, const xson_view& view)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, to_bsonview(view)));
}

void xson_builder::append_array(std::function<void(xson_builder&)> func)
{
	auto builder = to_subarray(m_builder);
	builder->append([this, func](bsoncxx::builder::basic::sub_array sub) {
		xson_builder sub_builder(true, true, &sub);
		func(sub_builder);
	});
}

void xson_builder::append_array(const std::string& key, std::function<void(xson_builder&)> func)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, [this, func](bsoncxx::builder::basic::sub_array sub) {
		xson_builder sub_builder(true, false, &sub);
		func(sub_builder);
	}));
}

void xson_builder::append_document(std::function<void(xson_builder&)> func)
{
	auto builder = to_subarray(m_builder);
	builder->append([this, func](bsoncxx::builder::basic::sub_document sub) {
		xson_builder sub_builder(true, false, &sub);
		func(sub_builder);
	});
}

void xson_builder::append_document(const std::string& key, std::function<void(xson_builder&)> func)
{
	auto builder = to_subdocument(m_builder);
	builder->append(bsoncxx::builder::basic::kvp(key, [this, func](bsoncxx::builder::basic::sub_document sub) {
		xson_builder sub_builder(true, false, &sub);
		func(sub_builder);
	}));
}

