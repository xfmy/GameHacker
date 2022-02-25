// PeocessShow.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "PeocessShow.h"
#include "afxdialogex.h"

// CPeocessShow 对话框

IMPLEMENT_DYNAMIC(CPeocessShow, CDialogEx)

CPeocessShow::CPeocessShow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
{
}

CPeocessShow::~CPeocessShow()
{
}

void CPeocessShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CPeocessShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	long istyle = GetWindowLongPtr(m_list.m_hWnd, GWL_STYLE);
	istyle |= LVS_REPORT;
	SetWindowLongPtr(m_list.m_hWnd, GWL_STYLE, istyle);
	DWORD dstyle = m_list.GetExtendedStyle();
	dstyle |= LVS_EX_FULLROWSELECT;
	dstyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dstyle);

	m_list.InsertColumn(0, L"模块名称", 0, 200);
	m_list.InsertColumn(1, L"基址", 0, 200);
	m_list.InsertColumn(2, L"大小", 0, 200);
	//m_list.InsertColumn(3, L"状态", 0, 200);
	m_list.InsertColumn(3, L"文件位置", 0, 600);

	return 0;
}

//void CPeocessShow::AddColumn(CString name, size_t len)
//{
//	m_list.InsertColumn(ColumnCount, name, 0, len);
//}
#include <TlHelp32.h>
void CPeocessShow::SetModList(CString pid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _ttoi(pid));
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modl{ sizeof(MODULEENTRY32) };
		BOOL BMORE = Module32First(hSnap, &modl);

		while (BMORE)
		{
			CString txt;
			txt.Format(L"%x", modl.hModule);
			m_list.InsertItem(0, modl.szModule);
			m_list.SetItemText(0, 1, txt);
			txt.Format(L"%x", modl.modBaseSize);
			m_list.SetItemText(0, 2, txt);
			m_list.SetItemText(0, 3, modl.szExePath);
			BMORE = Module32Next(hSnap, &modl);
		}
	}
}

BEGIN_MESSAGE_MAP(CPeocessShow, CDialogEx)
	// ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CPeocessShow::OnNMCustomdrawList1)
END_MESSAGE_MAP()

// CPeocessShow 消息处理程序

//void CPeocessShow::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	*pResult = 0;
//
//	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;
//	auto& cmd = lp->nmcd;
//	switch (cmd.dwDrawStage)
//	{
//	case CDDS_PREPAINT:
//		*pResult = CDRF_NOTIFYITEMDRAW;
//		break;
//	case CDDS_ITEMPREPAINT:
//		CString wTxt = contrast.m_List.
//	default:
//		break;
//	}
//}