#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "lw_ui_lib.h"

class LWUI_API CDragTabCtrl : public CTabCtrl
{
public:
	CDragTabCtrl();
	~CDragTabCtrl();

public:
	void SetDragable(BOOL enable);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);



protected:
	BOOL m_bDragable;
};

