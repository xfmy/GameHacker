#pragma once

// CObjectInfo 对话框

class CObjectInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectInfo)

public:
	CObjectInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CObjectInfo();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 内存地址
	CString m_MenoryAddress;
	// 对象大小
	CString m_objectSize;
	// 对象名称
	CString m_objectName;
	// 注释
	CString m_Info;
};
