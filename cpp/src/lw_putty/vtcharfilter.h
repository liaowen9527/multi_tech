#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include "lw_putty_dll.h"
#include "vtcallback.h"
#include "vtterminal.h"


class LWPutty_API CVtCharFilter : public CVtCallback
{
public:
    CVtCharFilter();
    virtual ~CVtCharFilter();

public:
	CVtTerminal& GetTerminal();

    void Parse(const char* str, int len);

	void GetClipLines(std::vector<VtLine>& lines, bool bSwap);
	void ClearClipLines();

protected:
	virtual void OnClipLine(void* termline) override;

protected:
	CVtTerminal m_terminal;
	std::vector<VtLine> m_clipLines;
};

typedef std::shared_ptr<CVtCharFilter>  CVtCharFilterPtr;

