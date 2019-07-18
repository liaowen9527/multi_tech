#pragma once
#include "lw_ui_lib.h"

class LWUI_API CDrawTextProcessor
{
public:
	CDrawTextProcessor();
	~CDrawTextProcessor();

	CDC* GetDC();
	void SetDC(CDC* pDC);

	CRect GetTextRect();
	void SetTextRect(const CRect& rcRect);

	int GetRowHeight();
	int RecalcRowHeight(CDC* pDC, CFont* pFont);

	int GetRowsCount(BOOL bWithPartlyVisible);

	int GetTabSize();
	void SetTabSize(int nTabSize);

	int GetScrollXOffset();
	void SetScrollXOffset(int nOffsetX);

	const TEXTMETRIC& GetTextMetrics();

	int GetSpaceWidth();

	BOOL HitTestRow(int y, int& rnRow);
	BOOL HitTestCol(const CString& strLine, int x, int& rnCol);
	int GetColPosX(const CString& strLine, int nCol);

protected:
	CDC* m_pDC;

	CRect m_rcTextRect;
	TEXTMETRIC m_tmText;
	int m_nRowHeight;
	int m_nSpaceWidth;

	int m_nTabSize;
	int m_nScrollXOffset;

	BOOL m_bUseOutputDC;
};


