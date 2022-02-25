// ObjectInfo.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "ObjectInfo.h"
#include "afxdialogex.h"

// CObjectInfo 对话框

IMPLEMENT_DYNAMIC(CObjectInfo, CDialogEx)

CObjectInfo::CObjectInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROINFO_DIALOG, pParent)
	, m_MenoryAddress(_T(""))
	, m_objectSize(_T(""))
	, m_objectName(_T(""))
	, m_Info(_T(""))
{
}

CObjectInfo::~CObjectInfo()
{
}

void CObjectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MenoryAddress);
	DDX_Text(pDX, IDC_EDIT2, m_objectSize);
	DDX_Text(pDX, IDC_EDIT3, m_objectName);
	DDX_Text(pDX, IDC_EDIT4, m_Info);
}

BEGIN_MESSAGE_MAP(CObjectInfo, CDialogEx)
END_MESSAGE_MAP()

// CObjectInfo 消息处理程序