#pragma once

// ADDGAME 对话框

class ADDGAME : public CDialogEx
{
	DECLARE_DYNAMIC(ADDGAME)

public:
	ADDGAME(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ADDGAME();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDGAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	// 游戏名称
	CString gameName;
	// 游戏路径
	CString gamePath;
	afx_msg void OnBnClickedButton2();
	CString dllFile;
	CString curPathName;
	CString gameCommandParameter;
	afx_msg void OnBnClickedButton3();
	bool BL = false;
	int index = 0;
};
