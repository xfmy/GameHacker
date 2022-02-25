#pragma once

// CContrast 对话框

class CContrast : public CDialogEx
{
	DECLARE_DYNAMIC(CContrast)

public:
	CContrast(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CContrast();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTRAST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
};
