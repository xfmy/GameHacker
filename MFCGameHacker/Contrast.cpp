// Contrast.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "Contrast.h"
#include "afxdialogex.h"

// CContrast 对话框

IMPLEMENT_DYNAMIC(CContrast, CDialogEx)

CContrast::CContrast(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTRAST_DIALOG, pParent)
{
}

CContrast::~CContrast()
{
}

void CContrast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BOOL CContrast::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	long istyle = GetWindowLongPtr(m_List.m_hWnd, GWL_STYLE);
	istyle |= LVS_REPORT;
	SetWindowLongPtr(m_List.m_hWnd, GWL_STYLE, istyle);
	DWORD dstyle = m_List.GetExtendedStyle();
	dstyle |= LVS_EX_FULLROWSELECT;
	dstyle |= LVS_EX_GRIDLINES;
	m_List.SetExtendedStyle(dstyle);

	//插入列
	m_List.InsertColumn(0, L"模块名称", 0, 200);
	m_List.InsertColumn(1, L"基址", 0, 200);
	m_List.InsertColumn(2, L"大小", 0, 200);
	m_List.InsertColumn(3, L"状态", 0, 200);
	m_List.InsertColumn(4, L"文件位置", 0, 600);

	return 0;
}

BEGIN_MESSAGE_MAP(CContrast, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CContrast::OnNMCustomdrawList1)
END_MESSAGE_MAP()

// CContrast 消息处理程序

void CContrast::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;
	auto& cmd = lp->nmcd;
	switch (cmd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
	{
		CString wTxt = m_List.GetItemText(cmd.dwItemSpec, 4);
		if (wTxt == L"增加")
		{
			lp->clrText = RGB(255, 0, 0);
			lp->clrTextBk = RGB(0, 255, 0);
		}
		else if (wTxt == L"减少")
		{
			lp->clrText = RGB(0, 0, 255);
		}
	}
	*pResult = CDRF_DODEFAULT;
	break;
	}
}