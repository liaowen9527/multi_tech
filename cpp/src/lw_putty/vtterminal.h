#pragma once
#include <string>
#include <vector>
#include "vtline.h"
#include "lw_putty_dll.h"

class CVtCallback;

class LWPutty_API CVtTerminal
{
public:
	CVtTerminal();
	~CVtTerminal();

public:
	void SetCallback(CVtCallback* pCallback);
	void Init(int rows, int cols);

	void GetCursorPos(int& x, int& y);
	bool IsAlternateScreen();
	void GetBuffer(std::vector<VtLine>& vecBuffer);

	void GetTermLine(void* line, VtLine& vtLine);

public:
	int TermData(const char *data, int len);

protected:
	bool HasCrLf(void* line);

protected:
	void* m_term;
	CVtCallback* m_pCallback;
};