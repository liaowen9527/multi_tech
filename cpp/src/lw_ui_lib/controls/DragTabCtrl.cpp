#include "DragTabCtrl.h"
#include <algorithm>



CDragTabCtrl::CDragTabCtrl()
	: m_bDragable(FALSE)
{

}

CDragTabCtrl::~CDragTabCtrl()
{

}

BEGIN_MESSAGE_MAP(CDragTabCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CDragTabCtrl::SetDragable(BOOL enable)
{
	m_bDragable = enable;
}

void CDragTabCtrl::OnPaint()
{
	__super::OnPaint();
}

void CDragTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void CDragTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

void CDragTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);
}

void CDragTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

void CDragTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	__super::DrawItem(lpDrawItemStruct);
}