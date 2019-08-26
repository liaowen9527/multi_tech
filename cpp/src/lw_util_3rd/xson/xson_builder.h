#pragma once
#include "xson.h"

#include "lw_util_3rd.h"


class LWUtil3rd_API xson_builder
{
public:
	xson_builder();
	~xson_builder();

protected:
	xson_builder(bool bSub, bool bArray, void* builder);

public:
	inline xson extract();
	inline xson_view view();

	inline void clear();

	inline void append(int val);
	inline void append(const std::string& key, int val);

	inline void append(double val);
	inline void append(const std::string& key, double val);

	inline void append(const char* str);
	inline void append(const std::string& key, const char* str);

	inline void append(const std::string& val);
	inline void append(const std::string& key, const std::string& val);

	inline void append(bool val);
	inline void append(const std::string& key, bool val);

	inline void append(time_t msec);
	inline void append(const std::string& key, time_t msec);

	inline void append(const xson& doc);
	inline void append(const std::string& key, const xson& doc);

	inline void append(const xson_view& view);
	inline void append(const std::string& key, const xson_view& view);

	inline void append_array(std::function<void(xson_builder&)> func);
	inline void append_array(const std::string& key, std::function<void(xson_builder&)> func);

	inline void append_document(std::function<void(xson_builder&)> func);
	inline void append_document(const std::string& key, std::function<void(xson_builder&)> func);

protected:
	void* m_builder;
	bool m_bArray;
	bool m_bSub;
};

