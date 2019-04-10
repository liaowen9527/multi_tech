// MainFrm.h : interface of the CMainFrame class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2011 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BB6DDD0D_D9DA_4314_AAFC_EC8012C079B7__INCLUDED_)
#define AFX_MAINFRM_H__BB6DDD0D_D9DA_4314_AAFC_EC8012C079B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPCaptionThemeNativeWinXP : public CXTPCaptionTheme
{
protected:
	void DrawCaptionBack(CDC* pDC, CXTPCaption* /*pCaption*/, CRect& rc)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rc.left, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetBorderColor(true));
		pDC->SetBkMode(TRANSPARENT);
	}
};
//

class CSplitterWndEx : public CXTPSplitterWnd
{
public:
	CSplitterWndEx()
	{
		m_cyBorder = m_cxBorder = 2;
		m_cxSplitter = m_cySplitter = 4;
		m_cySplitterGap = m_cxSplitterGap = 4;

		m_cxBorderShare = m_cyBorderShare = 0;



		m_bFlatSplit = TRUE;
		m_dwxStyle = XTP_SPLIT_NOFULLDRAG|XTP_SPLIT_NOBORDER;
		m_bFullDrag = FALSE;
	}

};

class CItemTreeView;
class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	void UpdateCaption(LPCTSTR lpszText, HICON hIcon);

	XTPPaintTheme m_nTheme;
	void SetTheme(XTPPaintTheme nTheme);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	bool             m_bShowPreview;
	bool             m_bDestroy;
	CXTPStatusBar        m_wndStatusBar;
	CImageList       m_ilLargeIcons;
	CImageList       m_ilSmallIcons;
	CXTPOfficeBorder<CXTPTaskPanel>  m_wndOutlookBar;
	CSplitterWndEx m_wndSplitter1;
	CSplitterWndEx   m_wndSplitter2;
	CSplitterWndEx   m_wndSplitter3;
	CSplitterWndEx   m_wndSplitter4;
	CXTPTreeCtrl*     m_pwndTreeCtrl;
	CItemTreeView*   m_pItemTreeView;
	CXTPCaption       m_wndCaption;
	CFont            m_fontCaption;
	CXTPComboBox  m_wndFindCombo;
	UINT			 m_nFrameworkSkin;

	BOOL InitializeOutlookBar();
	BOOL InitializeCaptionBar();
	void CloseTreeViewPane();
	bool InitializeStandardBar();
	void UpdateButtonStyle(CXTPCaptionButton& btnCaption);
	void UpdateSplitterStyle(CXTPSplitterWnd& wndSplitter);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDoNothing();
	afx_msg void OnFilePreviewpane();
	afx_msg void OnUpdateFilePreviewpane(CCmdUI* pCmdUI);
	afx_msg void OnSysColorChange();
	afx_msg void OnClose();
	afx_msg void OnViewThemeOfficexp();
	afx_msg void OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI);
	afx_msg void OnViewThemeOffice2003();
	afx_msg void OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnViewThemeOffice2000();
	afx_msg void OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI);
	afx_msg void OnViewThemeWinXP();
	afx_msg void OnUpdateSkin(CCmdUI* pCmdUI);

	//}}AFX_MSG
	void OnUpdateViewThemeWinXP(CCmdUI* pCmdUI);

	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnCustomize();
	afx_msg void OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnShortcutLBoxNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPushPinButton();
	afx_msg void OnPushPinCancel();
	void OnOutbarRButtonDown(CXTPTaskPanelItem* pItem);
	void OnRenameItem(CXTPTaskPanelItem* pItem);
	DECLARE_MESSAGE_MAP()
	void OnSkinPopup();

	void OnSkinChanged(UINT nID);
	CString GetStylesPath();

	friend class CItemTreeView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BB6DDD0D_D9DA_4314_AAFC_EC8012C079B7__INCLUDED_)
