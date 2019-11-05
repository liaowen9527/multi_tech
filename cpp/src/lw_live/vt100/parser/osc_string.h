#pragma once
#include <vector>

class osc_string
{
public:
	osc_string();
	~osc_string();

public:
	bool is_osc_w();
	void set_osc_w(bool val);

	int length();
	void resize(int len);

	void append(char c);

protected:
	bool m_osc_w;
	std::string m_str;
};