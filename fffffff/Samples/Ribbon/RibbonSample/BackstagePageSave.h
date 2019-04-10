#if !defined(AFX_CBackstagePageSave_H__0789D950_3894_46E8_B34F_996A19A138E3__INCLUDED_)
#define AFX_CBackstagePageSave_H__0789D950_3894_46E8_B34F_996A19A138E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBackstagePageSave.h : header file
//
#include "BackstagePageSaveEmail.h"
#include "BackstagePageSaveWeb.h"

/////////////////////////////////////////////////////////////////////////////
// CBackstagePageSave dialog

class CBackstagePageSave : public CXTPRibbonBackstagePage
{
// Construction
public:
	CBackstagePageSave();
	~CBackstagePageSave();

// Dialog Data
	//{{AFX_DATA(CBackstagePageSave)
	enum { IDD = IDD_BACKSTAGEPAGE_SAVE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBackstagePageSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL OnInitDialog();
// Implementation

	CBackstagePageSaveEmail m_pageSaveEmail;
	CBackstagePageSaveWeb m_pageSaveWeb;


	CXTPRibbonBackstageSeparator m_lblSeparator1;
	CXTPRibbonBackstageSeparator m_lblSeparator2;
	CXTPRibbonBackstageSeparator m_lblSeparator4;

	CXTPRibbonBackstageLabel m_lblSaveAndSend;
	CXTPRibbonBackstageLabel m_lblFileTypes;

	CXTPRibbonBackstageButton m_btnSendEmail;
	CXTPRibbonBackstageButton m_btnSaveToWeb;
	CXTPRibbonBackstageButton m_btnSaveToSharePoint;
	CXTPRibbonBackstageButton m_btnPublish;
	CXTPRibbonBackstageButton m_btnChangeFileType;
	CXTPRibbonBackstageButton m_btnCreatePDF;

	CXTPRibbonBackstageButton* m_pSelectedButton;
	
	void InitButton(CXTPRibbonBackstageButton& btn);
	
	CXTPRibbonBackstagePage* GetButtonPage(CXTPRibbonBackstageButton* pButton);
	CXTPImageManager m_imagelist;


protected:
	// Generated message map functions
	//{{AFX_MSG(CBackstagePageSave)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	void OnButtonSelected();
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBackstagePageSave_H__0789D950_3894_46E8_B34F_996A19A138E3__INCLUDED_)
