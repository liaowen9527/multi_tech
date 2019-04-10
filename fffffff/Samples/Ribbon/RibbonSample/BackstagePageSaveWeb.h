#if !defined(AFX_BACKSTAGEPAGESAVEWEB_H__025EFF2E_691E_469B_A5B5_95A48731C7D8__INCLUDED_)
#define AFX_BACKSTAGEPAGESAVEWEB_H__025EFF2E_691E_469B_A5B5_95A48731C7D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackstagePageSaveWeb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackstagePageSaveWeb dialog

class CBackstagePageSaveWeb : public CXTPRibbonBackstagePage
{
	DECLARE_DYNCREATE(CBackstagePageSaveWeb)

// Construction
public:
	CBackstagePageSaveWeb();
	~CBackstagePageSaveWeb();

// Dialog Data
	//{{AFX_DATA(CBackstagePageSaveWeb)
	enum { IDD = IDD_BACKSTAGEPAGE_SAVE_WEB };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBackstagePageSaveWeb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	CXTPRibbonBackstageLabel m_lblCaption;
	CXTPRibbonBackstageSeparator m_lblSeparator;
	CXTPRibbonBackstageButton m_btnSignIn;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBackstagePageSaveWeb)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKSTAGEPAGESAVEWEB_H__025EFF2E_691E_469B_A5B5_95A48731C7D8__INCLUDED_)
