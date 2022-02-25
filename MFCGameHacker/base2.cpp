// base2.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "base2.h"
#include "afxdialogex.h"

// base2 对话框

IMPLEMENT_DYNAMIC(base2, CDialogEx)

base2::base2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

base2::~base2()
{
}

void base2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BOOL base2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	addDialog(&cps[0], IDD_PROCESS_DIALOG, L"正常进程", true);
	addDialog(&cps[1], IDD_PROCESS_DIALOG, L"感染进程", true);
	addDialog(&contrast, IDD_CONTRAST_DIALOG, L"进程分析", true);
	return 0;
}

BEGIN_MESSAGE_MAP(base2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &base2::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &base2::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON2, &base2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &base2::OnBnClickedButton4)
END_MESSAGE_MAP()

// base2 消息处理程序

void base2::OnBnClickedButton1()
{
	cps[0].m_list.DeleteAllItems();
	pList.DoModal();
	GetDlgItem(IDC_EDIT1)->SetWindowText(pList.proName);
	cps[0].SetModList(pList.pid);
}

bool base2::addDialog(CDialogEx* obj, int IDC, CString name, bool isShow)
{
	if (link > MAX_DLGNUM)
		return false;
	dlg[link] = obj;
	obj->Create(IDC, &m_Tab);
	dlg[link]->ShowWindow(isShow);
	CRect re;
	m_Tab.GetClientRect(&re);
	re.bottom -= 30;
	dlg[link]->MoveWindow(&re);
	m_Tab.InsertItem(link, name);

	link++;
	return true;
}

void base2::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;

	CRect re;
	m_Tab.GetClientRect(&re);
	re.bottom -= 30;

	for (int i = 0; i < link; i++)
	{
		if (dlg[i])
			dlg[i]->MoveWindow(&re);
	}
}

void base2::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	int cur = m_Tab.GetCurSel();
	for (int i = 0; i < link; i++)
	{
		dlg[i]->ShowWindow(i == cur);
	}
}

void base2::OnBnClickedButton2()
{
	//pList.proList.DeleteAllItems();
	cps[1].m_list.DeleteAllItems();
	pList.DoModal();
	GetDlgItem(IDC_EDIT2)->SetWindowText(pList.proName);
	cps[1].SetModList(pList.pid);
}

void base2::OnBnClickedButton4()
{
	contrast.m_List.DeleteAllItems();
	int count1 = cps[0].m_list.GetItemCount();
	int count2 = cps[1].m_list.GetItemCount();
	if (count1 && count2)//二者都不为空
	{
		for (int i = 0; i < count1; i++)
		{
			CString txt = cps[0].m_list.GetItemText(i, 3);
			for (int j = 0; j < count2; j++)
			{
				if (cps[1].m_list.GetItemText(j, 3).MakeUpper() == txt.MakeUpper())
				{
					cps[1].m_list.SetItemData(j, 1);
					cps[0].m_list.SetItemData(i, 1);
					break;
				}
			}
		}
		CString wTxt[2]{ L"减少",L"正常" };
		for (int i = 0; i < count1; i++) {
			DWORD state = cps[0].m_list.GetItemData(i);
			contrast.m_List.InsertItem(0, cps[0].m_list.GetItemText(i, 0));
			contrast.m_List.SetItemText(0, 1, cps[0].m_list.GetItemText(i, 1));
			contrast.m_List.SetItemText(0, 2, cps[0].m_list.GetItemText(i, 2));
			contrast.m_List.SetItemText(0, 3, cps[0].m_list.GetItemText(i, 3));
			contrast.m_List.SetItemText(0, 4, wTxt[state]);
		}
		for (int i = 0; i < count2; i++)
		{
			DWORD state = cps[1].m_list.GetItemData(i);
			if (state == 0)
			{
				contrast.m_List.InsertItem(0, cps[1].m_list.GetItemText(i, 0));
				contrast.m_List.SetItemText(0, 1, cps[1].m_list.GetItemText(i, 1));
				contrast.m_List.SetItemText(0, 2, cps[1].m_list.GetItemText(i, 2));
				contrast.m_List.SetItemText(0, 3, cps[1].m_list.GetItemText(i, 3));
				contrast.m_List.SetItemText(0, 4, L"增加");
			}
		}
	}
}