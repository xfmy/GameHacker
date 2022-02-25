// MFCGameHackerDlg.h: 头文件
//

#pragma once
#include "base1.h"
#include "base2.h"
#include "GameAnalyse.h"
#define MAX_DLG_NUM 5//最大对话框
// CMFCGameHackerDlg 对话框
class CMFCGameHackerDlg : public CDialogEx
{
	// 构造
public:
	CMFCGameHackerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CDialogEx* dlg[MAX_DLG_NUM];//对话框数组
	int link = 0;//对话框数量
	bool addDialog(CDialogEx*, int, CString, bool);//添加子窗口
	base1 base_1;
	base2 base_2;
	CGameAnalyse m_gameAnalyse;
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCGAMEHACKER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
