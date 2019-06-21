#include "TerminalPaintManager.h"
#include "Terminal.h"

namespace lw_ui
{

	TerminalPaintManager::TerminalPaintManager(CTerminal* pTerminal/* = NULL*/)
	{
		m_pTerminal = pTerminal;
		if (!m_pTerminal)
		{
			abort();
		}
	}

	TerminalPaintManager::~TerminalPaintManager()
	{

	}

	void TerminalPaintManager::SetFont(const LOGFONT& logfont)
	{
		if (m_font.m_hObject)
		{
			m_font.DeleteObject();
		}
		m_font.CreateFontIndirect(&logfont);
		m_nRowHeight = 0;
	}

	void TerminalPaintManager::SetFont(const CString& strFontName, int nFontSize)
	{
		LOGFONT logfont;
		::ZeroMemory(&logfont, sizeof(LOGFONT));

		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfWeight = FW_NORMAL;
		logfont.lfHeight = nFontSize;
		_tcscpy(logfont.lfFaceName, (LPCTSTR)strFontName);

		SetFont(logfont);
	}

	void TerminalPaintManager::DrawBackground(CDC* pDC, const CRect& rc)
	{
		pDC->FillSolidRect(rc, m_clrBack);
	}

	void TerminalPaintManager::DrawText(CDC* pDC)
	{
		RecalcRowHeight(pDC);

		TerminalDelegate* pDelegate = m_pTerminal->GetDelegate();
		if (NULL == pDelegate)
		{
			return;
		}

		int nFirstRow = 0;
		int nLastRow = 0;
		pDelegate->GetDispRows(nFirstRow, nLastRow);

		std::vector<TextBlock> vecBlock;
		pDelegate->GetLines(nFirstRow, nLastRow, vecBlock);

		for (const TextBlock& block : vecBlock)
		{

		}
	}

	void TerminalPaintManager::DrawCursor(CDC* pDC)
	{

	}

	void TerminalPaintManager::InitStyle()
	{
		m_clrBack = RGB(0, 0, 0);
		m_clrForeground = RGB(255, 255, 255);

		SetFont(_T("Courier New"), 9);
	}

	void TerminalPaintManager::RecalcRowHeight(CDC* pDC)
	{
		if (m_nRowHeight > 0)
		{
			return;
		}

		CFont* pOldFont = pDC->SelectObject(&m_font);

		TEXTMETRIC tmText;
		BOOL bRes = pDC->GetOutputTextMetrics(&tmText);
		m_nRowHeight = tmText.tmHeight;

		pDC->SelectObject(pOldFont);
	}

}

