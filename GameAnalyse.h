#pragma once

// CGameAnalyse 对话框
#include "ProcessList.h"
#include "ObjectInfo.h"
class CGameAnalyse : public CDialogEx
{
	DECLARE_DYNAMIC(CGameAnalyse)

public:
	CGameAnalyse(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameAnalyse();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALYSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CListCtrl m_list;

	CProcessList m_proList;
	CObjectInfo  m_objInfo;

	HANDLE hProcess;
	CString wAppPath;
	CString wAnlyPath;
	CString wAnlyData;
	void setAppPath(const wchar_t* path);
	//初始化加载
	void init();
	//遍历从文件加载数据到树控件
	void traverseGenerated(CString paperPath);

	afx_msg void OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On32776();
};
