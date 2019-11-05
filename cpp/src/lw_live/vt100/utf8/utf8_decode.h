#pragma once
#include <string>


class utf8_decode
{
public:
	utf8_decode();
	~utf8_decode();

public:
	void reset();

	bool append(char c);
	bool is_finish();

	wchar_t get_wchar();
	bool is_wide();

protected:
	int get_len_by_first_char(unsigned char c);

protected:
	std::string m_utf8;
	int m_size;                          /* The size of the UTF character. */
};