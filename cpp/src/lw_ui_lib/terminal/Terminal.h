#pragma once
#include <vector>
#include "TerminalPaintManager.h"
#include "lw_ui_dll.h"

namespace lw_ui
{

class LWUI_API TerminalDelegate
{
public:
	virtual void GetDispRows(int& nFirstRow, int& nLastRow) = 0;
	virtual void GetLines(int nFirstRow, int nLastRow,std::vector<TextBlock>& vecBlock) = 0;
	virtual void GetLine(int nTextRow, std::vector<TextBlock>& vecBlock) = 0;
};

class LWUI_API CTerminal : public CWnd
{
public:
	CTerminal();
	~CTerminal();

public:
	TerminalPaintManager* GetPaintManager();
	void SetPaintManager(TerminalPaintManager* pPaintManager);

	TerminalDelegate* GetDelegate();
	void SetDelegate(TerminalDelegate* pDelegate);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	TerminalPaintManager* m_pPaintManager;
	TerminalDelegate* m_pDelegate;

	int m_nDispRow;
};

}

