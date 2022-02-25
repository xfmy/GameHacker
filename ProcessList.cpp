// ProcessList.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "ProcessList.h"
#include "afxdialogex.h"

// CProcessList 对话框

IMPLEMENT_DYNAMIC(CProcessList, CDialogEx)

CProcessList::CProcessList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_LIST, pParent)
{
}

CProcessList::~CProcessList()
{
}

BOOL CProcessList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	long istyle = GetWindowLongPtr(proList.m_hWnd, GWL_STYLE);
	istyle |= LVS_REPORT;
	SetWindowLongPtr(proList.m_hWnd, GWL_STYLE, istyle);
	DWORD dstyle = proList.GetExtendedStyle();
	dstyle |= LVS_EX_FULLROWSELECT;
	dstyle |= LVS_EX_GRIDLINES;
	proList.SetExtendedStyle(dstyle);

	//插入列
	proList.InsertColumn(0, L"pid", 0, 100);
	proList.InsertColumn(1, L"文件名", 0, 400);

	NewProcessView();//获取进程列表
	return true;
}

void CProcessList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIDGET_LIST, proList);
}
#include <TlHelp32.h>
void CProcessList::NewProcessView()
{
	proList.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"获取进程列表失败");
		return;
	}
	PROCESSENTRY32 pe{ sizeof(pe) };
	BOOL BMORE = Process32First(hSnap, &pe);
	while (BMORE)
	{
		CString txt;
		txt.Format(L"%d", pe.th32ProcessID);
		proList.InsertItem(0, txt);
		proList.SetItemText(0, 1, pe.szExeFile);
		BMORE = Process32Next(hSnap, &pe);
	}
}

BEGIN_MESSAGE_MAP(CProcessList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_WIDGET_LIST, &CProcessList::OnNMDblclkWidgetList)
END_MESSAGE_MAP()

// CProcessList 消息处理程序
//左键双击
void CProcessList::OnNMDblclkWidgetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem != -1)
	{
		pid = proList.GetItemText(pNMItemActivate->iItem, 0);
		proName = proList.GetItemText(pNMItemActivate->iItem, 1);
		this->EndDialog(1);
	}
	*pResult = 0;
}