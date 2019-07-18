#include "FontDC.h"

#define COLORREF_NULL (COLORREF)-1

CFontDC::CFontDC(CDC* pDC, CFont* pFont /*= NULL*/)
{
	m_pDC = pDC;
	m_pOldFont = NULL;
	m_clrOldTextColor = COLORREF_NULL;

	if (pFont)
	{
		SetFont(pFont);
	}
}

CFontDC::CFontDC(CDC* pDC, CFont* pFont, COLORREF clrTextColor)
{
	m_pDC = pDC;
	m_pOldFont = NULL;
	m_clrOldTextColor = COLORREF_NULL;

	if (pFont)
	{
		SetFont(pFont);
	}

	SetColor(clrTextColor);
}

CFontDC::~CFontDC()
{
	ReleaseFont();
	ReleaseColor();
}

void CFontDC::SetFont(CFont* pFont)
{
	if (m_pDC && pFont)
	{
		CFont* pFontPrev = m_pDC->SelectObject(pFont);

		if (!m_pOldFont && pFontPrev)
		{
			m_pOldFont = pFontPrev;
		}
	}
}

void CFontDC::SetColor(COLORREF clrTextColor)
{
	if (m_pDC && clrTextColor != COLORREF_NULL)
	{
		COLORREF clrTextColorPrev = m_pDC->SetTextColor(clrTextColor);

		if (m_clrOldTextColor == COLORREF_NULL)
		{
			m_clrOldTextColor = clrTextColorPrev;
		}
	}
}

void CFontDC::SetFontColor(CFont* pFont, COLORREF clrTextColor)
{
	SetFont(pFont);
	SetColor(clrTextColor);
}

void CFontDC::ReleaseFont()
{
	if (m_pDC && m_pOldFont)
	{
		m_pDC->SelectObject(m_pOldFont);
		m_pOldFont = NULL;
	}
}

void CFontDC::ReleaseColor()
{
	if (m_pDC && m_clrOldTextColor != COLORREF_NULL)
	{
		m_pDC->SetTextColor(m_clrOldTextColor);
		m_clrOldTextColor = COLORREF_NULL;
	}
}
