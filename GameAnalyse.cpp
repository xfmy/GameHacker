// GameAnalyse.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "GameAnalyse.h"
#include "afxdialogex.h"

// CGameAnalyse 对话框

IMPLEMENT_DYNAMIC(CGameAnalyse, CDialogEx)

CGameAnalyse::CGameAnalyse(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANALYSE_DIALOG, pParent)
{
}

CGameAnalyse::~CGameAnalyse()
{
}

void CGameAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CGameAnalyse::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	long istyle = GetWindowLongPtr(m_list.m_hWnd, GWL_STYLE);
	istyle |= LVS_REPORT;
	SetWindowLongPtr(m_list.m_hWnd, GWL_STYLE, istyle);
	DWORD dstyle = m_list.GetExtendedStyle();
	dstyle |= LVS_EX_FULLROWSELECT;
	dstyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dstyle);

	m_list.InsertColumn(0, L"内存地址", 0, 100);
	m_list.InsertColumn(1, L"偏移", 0, 200);
	m_list.InsertColumn(2, L"类型", 0, 200);
	m_list.InsertColumn(3, L"名称", 0, 200);
	m_list.InsertColumn(3, L"值", 0, 200);
	m_list.InsertColumn(3, L"注释", 0, 200);
	return 0;
}

BEGIN_MESSAGE_MAP(CGameAnalyse, CDialogEx)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CGameAnalyse::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CGameAnalyse::OnNMRClickTree1)
	ON_COMMAND(ID_32776, &CGameAnalyse::On32776)
END_MESSAGE_MAP()

// CGameAnalyse 消息处理程序

//void CGameAnalyse::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	*pResult = 0;
//
//	DWORD dwpos = GetMessagePos();
//	CPoint point(LOWORD(dwpos), HIWORD(dwpos));
//	CMenu menu;
//	menu.LoadMenuW(IDR_MENU2);
//	CMenu* pop = menu.GetSubMenu(0);
//	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
//}

void CGameAnalyse::setAppPath(const wchar_t* path)
{
	wAppPath = path;
	wAnlyPath = wAppPath + L"\\RAN\\";
}

//初始化列表
void CGameAnalyse::init()
{
	WCHAR ch[MAX_PATH]{};
	GetModuleFileName(NULL, ch, MAX_PATH);
	(StrRChrW(ch, NULL, '\\'))[0] = 0;
	setAppPath(ch);

	if (!PathIsDirectory(wAnlyPath))
	{
		if (!CreateDirectory(wAnlyPath, NULL))
		{
			AfxMessageBox(L"创建RAN文件夹失败");
			return;
		}
	}

	traverseGenerated(wAnlyPath);
}

void CGameAnalyse::traverseGenerated(CString paperPath)
{
	m_tree.DeleteAllItems();
	WIN32_FIND_DATA fileData;
	HANDLE hFind = FindFirstFile(paperPath + L"*.ini", &fileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		(StrRStrIW(fileData.cFileName, NULL, L".ini"))[0] = 0;
		auto hRoot = m_tree.InsertItem(fileData.cFileName, TVI_ROOT);
		auto hBase = m_tree.InsertItem(L"基本信息", hRoot);

		m_tree.InsertItem(L"内存地址", hBase);
		m_tree.InsertItem(L"对象大小", hBase);
		m_tree.InsertItem(L"注释", hBase);

		m_tree.InsertItem(L"成员变量", hRoot);
		m_tree.InsertItem(L"成员函数", hRoot);

		if (!FindNextFile(hFind, &fileData))	break;
	}
}

void CGameAnalyse::OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	DWORD index = 0;
	if (hProcess)
	{
		auto hSel = m_tree.GetSelectedItem();
		auto hRoot = m_tree.GetRootItem();
		if (hSel == hRoot)
		{
			index = 1;
		}
		else index = 0;
	}

	DWORD dwpos = GetMessagePos();
	CPoint point(LOWORD(dwpos), HIWORD(dwpos));
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* pop = menu.GetSubMenu(index);
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

//加载游戏
void CGameAnalyse::On32776()
{
	//选择进程
	if (m_proList.DoModal() != IDCANCEL) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, _ttoi(m_proList.pid));
		if (hProcess)
		{
			auto troot = m_tree.InsertItem(m_proList.proName, TVI_ROOT);
			m_tree.InsertItem(L"公共数据", troot);
			m_tree.InsertItem(L"公共函数", troot);
			m_tree.InsertItem(L"HOOK分析", troot);

			wAnlyData = wAnlyPath + m_proList.proName + L".ini";

			int iSize{};

			char fileName[MAX_PATH]{};
			//返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
			iSize = WideCharToMultiByte(CP_ACP, 0, wAnlyData.GetBuffer(), -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, wAnlyData.GetBuffer(), -1, fileName, iSize, NULL, NULL);
			FILE* fi = fopen(fileName, "r");
			if (fi == NULL)
			{
				fi = fopen(fileName, "a+");
				if (fi == NULL) {
					CString errInfo = L"创建文件夹失败";
					errInfo.Format(L"%s%d", L"创建文件夹失败,错误码=", errno);
					AfxMessageBox(errInfo);
				}
				else fclose(fi);
			}
			else fclose(fi);
			traverseGenerated(wAnlyPath);
		}
		else {
			AfxMessageBox(L"函数加载失败");
		}
	}

	//填写进程对象信息
	m_objInfo.DoModal();
	//m_objInfo.UpdateData(TRUE);
}