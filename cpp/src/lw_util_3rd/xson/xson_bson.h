#pragma once

#include <bsoncxx/types.hpp>

#include "xson.h"
#include "xson_bson.h"

inline xson to_xson(bsoncxx::document::value& temp)
{
	auto len = temp.view().length();

	bsoncxx::document::value* p = new bsoncxx::document::value(temp.release(), len);
	xson ret;
	ret.bind(p);

	return ret;
}

//template<typename T>
inline xson_view to_xsonview(const bsoncxx::document::view& view)
{
	return xson_view((void*)view.data(), view.length());
}

inline bsoncxx::document::view to_bsonview(const xson& doc)
{
	bsoncxx::document::value* bson = (bsoncxx::document::value*)doc.data();
	return bson->view();
}

inline bsoncxx::document::view to_bsonview(const xson_view& doc)
{
	return bsoncxx::document::view((const uint8_t *)doc.data(), doc.length());
}

inline bsoncxx::document::element to_element(const xson_element& ele)
{
	return (*(bsoncxx::document::element*)(ele.data()));
}

inline xson_element to_element(const bsoncxx::array::element& ele)
{
	bsoncxx::array::element* temp = new bsoncxx::array::element(ele);

	return xson_element(std::shared_ptr<void>(temp));
}

inline xson_element to_element(const bsoncxx::document::element& ele)
{
	bsoncxx::document::element* temp = new bsoncxx::document::element(ele);

	return xson_element(std::shared_ptr<void>(temp));
}