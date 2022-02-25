#pragma once
#include "ProcessList.h"
#include "PeocessShow.h"
#include "Contrast.h"
#define MAX_DLGNUM 5//最大对话框
// base2 对话框

class base2 : public CDialogEx
{
	DECLARE_DYNAMIC(base2)

public:
	base2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~base2();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CPeocessShow cps[MAX_DLGNUM];
	CDialogEx* dlg[MAX_DLGNUM];//对话框数组
	WORD link = 0;//对话框数量

	afx_msg void OnBnClickedButton1();
	bool addDialog(CDialogEx* obj, int IDC, CString name, bool isShow = false);
	CProcessList pList;//窗口类
	CTabCtrl m_Tab;//选项卡
	CContrast contrast;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
