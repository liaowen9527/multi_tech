#pragma once

#define SAFE_DESTROY_WNDPTR(pWnd) \
	if(pWnd) \
{ \
	if(pWnd->GetSafeHwnd()) \
	pWnd->DestroyWindow(); \
	delete pWnd; \
	pWnd = NULL; \
}

#define SAFE_SHOWWINDOW(pWnd, nShowMode) \
	if(pWnd) \
{ \
	if(pWnd->GetSafeHwnd()) \
	pWnd->ShowWindow(nShowMode); \
}

#define SAFE_MOVEWINDOW(pWnd, rc) \
	if (pWnd && pWnd->GetSafeHwnd()) \
{ \
	pWnd->MoveWindow(rc); \
}

#define SAFE_MOVEWINDOW_IF_VISIBLE(pWnd, rc) \
	if (pWnd && pWnd->GetSafeHwnd() && pWnd->IsWindowVisible()) \
{ \
	pWnd->MoveWindow(rc); \
}

#define FULLCLIENTVIEW(pParentWnd, pChildWnd) \
	CRect rc; \
	pParentWnd->GetClientRect(&rc); \
	SAFE_MOVEWINDOW(pChildWnd, rc)

#define MAKEEQUALSIZE(pWnd, pRefWnd) \
	if(pRefWnd && pRefWnd->GetSafeHwnd()) \
{ \
	CRect rc; \
	pRefWnd->GetWindowRect(&rc); \
	ScreenToClient(&rc); \
	SAFE_MOVEWINDOW(pWnd, rc); \
}

#define POSET_RESTORESIZE(pWnd) \
	if(pWnd && pWnd->GetSafeHwnd()) \
{ \
	CRect rc; \
	pWnd->GetWindowRect(&rc); \
	pWnd->PostMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height())); \
}

#define SEND_RESTORESIZE(pWnd) \
	if(pWnd && pWnd->GetSafeHwnd()) \
{ \
	CRect rc; \
	pWnd->GetWindowRect(&rc); \
	pWnd->SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height())); \
}

template <class T>
T* FindParentWnd(CWnd* pCurWnd, bool bIncludeSelf = false)
{
	if (NULL == pCurWnd || NULL == pCurWnd->GetSafeHwnd())
	{
		return NULL;
	}

	T* pWndRet = NULL;
	CWnd* pWndTemp = bIncludeSelf ? pCurWnd : pCurWnd->GetParent();

	do
	{
		if (NULL == pWndTemp || NULL == pWndTemp->GetSafeHwnd())
		{
			break;
		}

		pWndRet = dynamic_cast<T*>(pWndTemp);
		//find
		if (NULL != pWndRet)
		{
			break;
		}

		pWndTemp = pWndTemp->GetParent();
	} while (1);

	return pWndRet;
}


namespace MFCWndHelper
{
	inline BOOL IsWindowVisible(CWnd* pWnd)
	{
		if (NULL == pWnd || NULL == pWnd->GetSafeHwnd())
		{
			return FALSE;
		}

		return (GetWindowLong(pWnd->GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE);
	}
}


