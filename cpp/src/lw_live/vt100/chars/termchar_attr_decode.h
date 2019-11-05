#pragma once

class termchar_attr_decode
{
public:
	termchar_attr_decode();
	~termchar_attr_decode();

public:
	unsigned long get_attr();
	void set_attr(unsigned long attr);

public:
	bool is_narrow();
	void enable_narrow(bool val);

	bool is_wide();
	void enable_wide(bool val);

	bool is_bold();
	void enable_bold(bool val);

	bool is_under();
	void enable_under(bool val);

	bool is_reverse();
	void enable_reverse(bool val);

	bool is_blink();
	void enable_blink(bool val);

	bool is_fgmask();
	void enable_fgmask(bool val);

	bool is_bgmask();
	void enable_bgmask(bool val);

	bool is_colours();
	void enable_colours(bool val);

	bool is_dim();
	void enable_dim(bool val);
	
protected:
	bool m_arrow;
	bool m_wide;
	bool m_bold;
	bool m_under;
	bool m_reverse;
	bool m_blink;
	bool m_fgmask;
	bool m_bgmask;
	bool m_colours;
	bool m_dim;

	int m_nfg;
	int m_nbg;
};
