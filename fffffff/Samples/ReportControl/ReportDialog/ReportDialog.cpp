// ReportDialog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ReportDialog.h"
#include "ReportDialogDlg.h"

#include "DatasetListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDialogApp

BEGIN_MESSAGE_MAP(CReportDialogApp, CWinApp)
	//{{AFX_MSG_MAP(CReportDialogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDialogApp construction

CReportDialogApp::CReportDialogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CReportDialogApp object

CReportDialogApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CReportDialogApp initialization

BOOL CReportDialogApp::InitInstance()
{
	AfxEnableControlContainer();
	CXTPWinDwmWrapper().SetProcessDPIAware();

	AfxOleInit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();         // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
#endif // MFC 6.0 or earlier

	CReportDialogDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = (int)dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
