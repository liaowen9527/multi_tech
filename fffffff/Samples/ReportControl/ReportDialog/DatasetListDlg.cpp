// DatasetListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReportDialog.h"
#include "DatasetListDlg.h"


// CDatasetListDlg dialog

IMPLEMENT_DYNAMIC(CDatasetListDlg, CDialog)

CDatasetListDlg::CDatasetListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDatasetListDlg::IDD, pParent),
	m_bIsVeryfySelected(false),
	m_selectedIndex(0),
	m_bShowMore(FALSE)
{

}

CDatasetListDlg::~CDatasetListDlg()
{
}

void CDatasetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATASET_CTRL, m_wndDataFolder);
}


BEGIN_MESSAGE_MAP(CDatasetListDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDatasetListDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_DATASET_CTRL, &CDatasetListDlg::OnBnDbClick)
	ON_NOTIFY(XTP_NM_REPORT_FOCUS_CHANGING, IDC_DATASET_CTRL, &CDatasetListDlg::OnReportFocusChanging)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_DATASET_CTRL, OnItemButtonClick)

	ON_BN_CLICKED(IDC_BUTTON_MORE, &CDatasetListDlg::OnBnClickedButtonMore)
END_MESSAGE_MAP()

class CFolderRecord:public CXTPReportRecord
{
	DECLARE_SERIAL(CFolderRecord)
public:
	CFolderRecord()
	{
		AddItem(new CXTPReportRecordItemText(""));
		AddItem(new CXTPReportRecordItemText(""));
	}
	CFolderRecord(CString name,CString date,int iconIndex)
	{
		m_data = name;
		CXTPReportRecordItem * item = AddItem(new CXTPReportRecordItemText(name));
		item->SetIconIndex(iconIndex);
		AddItem(new CXTPReportRecordItemText(date));
	}

	CString m_data;
};
IMPLEMENT_SERIAL(CFolderRecord, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

class CMoreBottonRecord:public CXTPReportRecord
{
	DECLARE_SERIAL(CMoreBottonRecord)
public:
	CMoreBottonRecord()
	{
		AddItem(new CXTPReportRecordItemText(""));
		CXTPReportRecordItem * pItem = AddItem(new CXTPReportRecordItemText(""));
		CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
		pButton->SetAlignment(xtpItemControlRight);
		pButton->SetCaption("More>  ");

		pButton->SetSize(CSize(60, 0));

		this->SetLocked(TRUE);
	}
};
IMPLEMENT_SERIAL(CMoreBottonRecord, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

// CDatasetListDlg message handlers

BOOL CDatasetListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_wndDataFolder.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//
	// load the image bitmap and set the image list for the 
	// report control.
	//

	// create the image list used by the report control.
	if (!m_ilIcons.Create(16, 15, ILC_COLOR24 | ILC_MASK, 0, 1))
		return -1;

	CBitmap bitmap;
	VERIFY(bitmap.LoadBitmap(IDB_BMREPORT));


	m_ilIcons.Add(&bitmap, RGB(255, 0, 255));
	m_wndDataFolder.SetImageList(&m_ilIcons);

	//
	//  Add sample columns
	//


	AddTitle();
	AddVeryfyItem();
	AddFolderItem();

	//m_wndDataFolder.AddRecord(new CFolderRecord(_T("hellohelhellllheheloHELLOHHHLLOOSFAFFSASFSDF"),_T("     2012-5-4"),0));
	//m_wndDataFolder.AddRecord(new CFolderRecord(_T("hello2"),_T("     2012-5-4"),0));

	//m_wndDataFolder.ShowHeader(FALSE);

	m_wndDataFolder.Populate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}



// CDataFolderListDlg message handlers

void CDatasetListDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDatasetListDlg::OnBnDbClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CXTPReportSelectedRows* rows = m_wndDataFolder.GetSelectedRows();
	rows->GetAt(0);

	CXTPReportRow* selectRow = m_wndDataFolder.GetFocusedRow();
	if(selectRow == NULL)
	{
		//m_selectFolder = 
		selectRow->GetRecord()->GetItem(0);
	}


	//OnOK();
}

void CDatasetListDlg::OnReportFocusChanging(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify) return;

	CXTPReportRow* pRowSel=pItemNotify->pRow;

	if (!pRowSel) return;

	m_selectedIndex = pRowSel->GetIndex();

	TRACE(_T("Click on row %d\n"),m_selectedIndex);

	CXTPReportRecord* pRecordSel=pRowSel->GetRecord();

	if(!pRecordSel) return;

	CXTPReportRecordItemText* pText=(CXTPReportRecordItemText*)(pRecordSel->GetItem(0));

	if(!pText) return;

	m_selectFolder = pText->GetValue().Trim();

}

void CDatasetListDlg::AddTitle()
{
	CString strDefTime = CTime::GetCurrentTime().Format(" %Y-%m-%d %H:%M ");
	CClientDC dc(&m_wndDataFolder); //Get the list ctrl client dc
	CSize szDefSecondCol = dc.GetTextExtent(strDefTime);//The 2nd Column is with default value: time 
	CRect rectListCtrl;
	m_wndDataFolder.GetWindowRect(rectListCtrl);
	//GetDlgItem(IDC_DATASET_CTRL)-> GetWindowRect(rectListCtrl);
	int nSecondColWidth = szDefSecondCol.cx;
	int nFirstColWidth = rectListCtrl.Width() - nSecondColWidth;
	if(nFirstColWidth <= 0)
	{
		nFirstColWidth = 160;
	}

	CXTPReportColumn*  title = m_wndDataFolder.AddColumn(new CXTPReportColumn(0, _T("Name"), nFirstColWidth));
	title->SetSortable(FALSE);
	title->EnableResize(FALSE);
	title->SetAllowDrag(FALSE);

	title = m_wndDataFolder.AddColumn(new CXTPReportColumn(1, _T("Last Update"), nSecondColWidth));
	title->SetSortable(FALSE);
	title->SetAllowDrag(FALSE);
	title->EnableResize(FALSE);
	m_wndDataFolder.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridNoLines);
}

void CDatasetListDlg::AddVeryfyItem()
{
	m_wndDataFolder.AddRecord(new CFolderRecord(_T("Verify Live"),_T(""),0));
}

void CDatasetListDlg::AddFolderItem()
{
	float height =0;
	for(int i=0;i<10;i++)
	{
		CString strDefTime = CTime::GetCurrentTime().Format(" %Y-%m-%d %H:%M ");
		CXTPReportRecord* m_record = m_wndDataFolder.AddRecord(new CFolderRecord(_T("Hello Heloo sfasfd"),strDefTime,1));

		//CRect rectListCtrl;
		//m_record->GetWindowRect(rectListCtrl);

		//height += rectListCtrl.Height();

	}

	//m_wndDataFolder.AddRecord(new CMoreBottonRecord());
}

void CDatasetListDlg::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT*)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;
	m_bShowMore = true;
	OnOK();
}

HBRUSH CDatasetListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何属性
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkColor(RGB(255,255,255));   
	HBRUSH b=CreateSolidBrush(RGB(255,255,255));
	return b;	

	if(nCtlColor==CTLCOLOR_BTN)          //更改按钮颜色
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));   
		HBRUSH b=CreateSolidBrush(RGB(255,255,255));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_SCROLLBAR)  //
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(233,233,220));
		HBRUSH b=CreateSolidBrush(RGB(233,233,220));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_EDIT)   //更改编辑框
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(165,254,236));
		HBRUSH b=CreateSolidBrush(RGB(165,254,236));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_STATIC)  //更改静态文本
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(166,254,1));
		HBRUSH b=CreateSolidBrush(RGB(166,254,1));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_DLG)   //更改对话框背景色
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(166,254,1));
		HBRUSH b=CreateSolidBrush(RGB(166,254,1));
		return b;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void CDatasetListDlg::OnBnClickedButtonMore()
{
	// TODO: Add your control notification handler code here

	m_bShowMore = true;
	OnOK();
}
