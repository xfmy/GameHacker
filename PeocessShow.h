#pragma once

// CPeocessShow 对话框

class CPeocessShow : public CDialogEx
{
	DECLARE_DYNAMIC(CPeocessShow)

public:
	CPeocessShow(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeocessShow();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	void SetModList(CString pid);
	//	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
};
