#pragma once


// CDatasetListDlg dialog

class CDatasetListDlg : public CDialog
{
	DECLARE_DYNAMIC(CDatasetListDlg)

public:
	CDatasetListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDatasetListDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATASET_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void AddTitle();
	void AddVeryfyItem();
	void AddFolderItem();

	void AddMoreBotton();

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnDbClick(NMHDR *,LRESULT *);
	afx_msg void OnReportFocusChanging(NMHDR* pNMHDR, LRESULT* result);
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT*);

	bool m_bIsVeryfySelected;
	int    m_selectedIndex;
	bool m_bShowMore;
private:
	CImageList m_ilIcons;
	CXTPReportControl m_wndDataFolder;
	CString m_selectFolder;
public:
	afx_msg void OnBnClickedButtonMore();
};
