#pragma once
#include <vector>

#define ARG_DEFAULT 0
#define ARGS_MAX 32 

class argument
{
public:
	argument();
	~argument();

public:
	int count();
	void resize(int n);

	unsigned int get_arg(int index);
	void set_arg(int index, unsigned int arg);
	void stream_in(char ch);

	char get_esc_query();
	void set_esc_query(char query);

public:
	unsigned int get_clamp_arg(int index, unsigned int max);
	unsigned int get_arg(int index, unsigned int _default);
	unsigned int get_arg(int index, unsigned int max, unsigned int _default);

protected:
	std::vector<unsigned int> m_esc_args;
	char m_esc_query;	//for combined command, such as Esc#8
};