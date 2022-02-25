#pragma once

// CProcessList 对话框

class CProcessList : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessList)

public:
	CProcessList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessList();
	virtual BOOL OnInitDialog();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 列表控件变量
	CListCtrl proList;
	CString proName;
	CString pid;
	void NewProcessView();

	afx_msg void OnNMDblclkWidgetList(NMHDR* pNMHDR, LRESULT* pResult);
};
