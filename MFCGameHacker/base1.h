#pragma once

// base1 对话框
#include "ADDGAME.h"
#include "INJECT.h"
class base1 : public CDialogEx
{
	DECLARE_DYNAMIC(base1)

public:
	base1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~base1();
	ADDGAME addGameDlg;
	void initWindow();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	INJECT m_inject;
	DECLARE_MESSAGE_MAP()
public:
	// 列表控件
	CListCtrl listCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtn();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	// 调试按钮
	BOOL Debug;
	// 是否注入
	BOOL IsInjext;
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void deleteItem();
	int index;
	bool IsSetItem = false;
	afx_msg void setItem();
	afx_msg void OnBnClickedCheck();
};
