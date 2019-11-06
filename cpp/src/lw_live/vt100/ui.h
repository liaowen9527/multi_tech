#pragma once

class ui
{
public:
	ui();
	~ui();

public:
	int get_rows();
	void set_rows(int rows);

	int get_cols();
	void set_cols(int cols);

	int get_disptop();
	void set_disptop(int top);

	int get_alt_sblines();
	void set_alt_sblines(int lines);

protected:
	int m_rows;
	int m_cols;

	int m_disptop;
	int m_alt_sblines;
};