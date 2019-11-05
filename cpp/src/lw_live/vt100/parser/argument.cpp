#include "argument.h"
#include <algorithm>


argument::argument()
{

}

argument::~argument()
{

}

int argument::count()
{
	return m_esc_args.size();
}

void argument::resize(int n)
{
	m_esc_args.resize(n);
}

unsigned int argument::get_arg(int index)
{
	while (m_esc_args.size() <= index)
	{
		m_esc_args.push_back(ARG_DEFAULT);
	}

	return m_esc_args[index];
}

void argument::set_arg(int index, unsigned int arg)
{
	while (m_esc_args.size() <= index)
	{
		m_esc_args.push_back(ARG_DEFAULT);
	}

	m_esc_args[index] = arg;
}

void argument::stream_in(char ch)
{
	int nargs = count();
	int index = nargs - 1;
	unsigned int arg = get_arg(index);
	int num = ch - '0';

	unsigned long long new_arg = arg;
	new_arg = 10 * new_arg + num;
	new_arg = std::min<unsigned long long>(new_arg, UINT_MAX);
	set_arg(index, new_arg);
}

char argument::get_esc_query()
{
	return m_esc_query;
}

void argument::set_esc_query(char query)
{
	m_esc_query = query;
}

unsigned int argument::get_clamp_arg(int index, unsigned int max)
{
	unsigned int arg = std::min<unsigned int>(get_arg(index), max);
	set_arg(index, arg);

	return arg;
}

unsigned int argument::get_arg(int index, unsigned int _default)
{
	unsigned int arg = get_arg(index);

	return arg == ARG_DEFAULT ? _default : arg;
}

unsigned int argument::get_arg(int index, unsigned int max, unsigned int _default)
{
	unsigned int arg = get_clamp_arg(index, max);

	return arg == ARG_DEFAULT ? _default : arg;
}

