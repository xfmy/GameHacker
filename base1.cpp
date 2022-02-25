// base1.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "base1.h"
#include "afxdialogex.h"

// base1 对话框

IMPLEMENT_DYNAMIC(base1, CDialogEx)

base1::base1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, Debug(FALSE)
	, IsInjext(FALSE)
{
}

base1::~base1()
{
}

void base1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listCtrl);
	DDX_Check(pDX, DEBUG_CHECK, Debug);
	DDX_Check(pDX, PUSH_CHECK, IsInjext);
}

BOOL base1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	long istyle = GetWindowLongPtr(listCtrl.m_hWnd, GWL_STYLE);
	istyle |= LVS_REPORT;
	SetWindowLongPtr(listCtrl.m_hWnd, GWL_STYLE, istyle);
	DWORD dstyle = listCtrl.GetExtendedStyle();
	dstyle |= LVS_EX_FULLROWSELECT;
	dstyle |= LVS_EX_GRIDLINES;
	listCtrl.SetExtendedStyle(dstyle);

	//插入列
	listCtrl.InsertColumn(0, L"名称", 0, 100);
	listCtrl.InsertColumn(1, L"可执行文件", 0, 400);
	listCtrl.InsertColumn(2, L"文件夹", 0, 200);
	listCtrl.InsertColumn(3, L"命令行", 0, 200);
	listCtrl.InsertColumn(4, L"注入模块", 0, 400);

	initWindow();
	return true;
}

BEGIN_MESSAGE_MAP(base1, CDialogEx)
	ON_BN_CLICKED(addGame_BTN, &base1::OnBnClickedBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &base1::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &base1::OnNMRClickList1)
	ON_COMMAND(ID_32771, &base1::deleteItem)
	ON_COMMAND(ID_32772, &base1::setItem)
	ON_BN_CLICKED(PUSH_CHECK, &base1::OnBnClickedCheck)
END_MESSAGE_MAP()

// base1 消息处理程序

void base1::OnBnClickedBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	addGameDlg.DoModal();
	initWindow();
}

void base1::initWindow()
{
	wchar_t strPath[MAX_PATH]{};
	GetCurrentDirectoryW(MAX_PATH, strPath);//获取当前路径，存放在strpath中，
	CString cpath = strPath;
	cpath += "\\GameData.ini";
	wchar_t strbuf[MAX_PATH]{};
	if (0 == GetPrivateProfileStringW(L"NUMBER", L"LINK", NULL, strbuf, MAX_PATH, cpath))
		WritePrivateProfileStringW(L"NUMBER", L"LINK", L"0", cpath);//文件不存在时，会自动创建
	else {
		int val = atoi((char*)strbuf);
		CString str;
		listCtrl.DeleteAllItems();
		for (int i = 0; i < val; i++)
		{
			CString gameName, gamePath, dllFile, curPathName, gameCommandParameter;
			str.Format(L"DATA_%d", i);
			GetPrivateProfileStringW(str, L"gameName", NULL, strbuf, MAX_PATH, cpath);
			gameName.Format(L"%s", strbuf);
			GetPrivateProfileStringW(str, L"curPathName", NULL, strbuf, MAX_PATH, cpath);
			curPathName.Format(L"%s", strbuf);
			GetPrivateProfileStringW(str, L"dllFile", NULL, strbuf, MAX_PATH, cpath);
			dllFile.Format(L"%s", strbuf);
			GetPrivateProfileStringW(str, L"gamePath", NULL, strbuf, MAX_PATH, cpath);
			gamePath.Format(L"%s", strbuf);
			GetPrivateProfileStringW(str, L"gameCommandParameter", NULL, strbuf, MAX_PATH, cpath);
			gameCommandParameter.Format(L"%s", strbuf);
			listCtrl.InsertItem(i, gameName);
			listCtrl.SetItemText(i, 1, curPathName);
			listCtrl.SetItemText(i, 2, gamePath);
			listCtrl.SetItemText(i, 3, gameCommandParameter);
			listCtrl.SetItemText(i, 4, dllFile);
		}
	}
}

void base1::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int index = pNMItemActivate->iItem;
	if (index < 0) return;
	CString gameName, gamePath, dllFile, curPathName, gameCommandParameter;

	curPathName = listCtrl.GetItemText(index, 1);
	gamePath = listCtrl.GetItemText(index, 2);
	gameCommandParameter = listCtrl.GetItemText(index, 3);
	dllFile = listCtrl.GetItemText(index, 4);

	PROCESS_INFORMATION mation{};
	m_inject.startProcess(gamePath, gameCommandParameter, curPathName, false, mation);
	HANDLE han = mation.hThread;
	DWORD pid = mation.dwProcessId;

	UpdateData(TRUE);
	//是否诸入
	if (IsInjext)
		m_inject.CreateRemoteCode(mation.hProcess, curPathName, dllFile.GetBuffer());
	ResumeThread(han);

	//启动OD
	if (!Debug)
	{
		return;
	}
	CString ODPath;//相对路径
	CString ODcurpath;//完整路径
	wchar_t flilepath[100];
	GetCurrentDirectoryW(100, flilepath);
	CString strPath{ flilepath };
	strPath += "\\GameData.ini";
	wchar_t buf[100]{};
	GetPrivateProfileStringW(L"OD", L"path", NULL, buf, 100, strPath);
	ODPath.Format(L"%s", buf);
	GetPrivateProfileStringW(L"OD", L"curPath", NULL, buf, 100, strPath);
	ODcurpath.Format(L"%s", buf);
	PROCESS_INFORMATION Odmation{};
	CString ODCmd;
	ODCmd.Format(L"\"%s\" -p %d", ODcurpath, pid);
	m_inject.startProcess(ODPath, ODCmd, ODcurpath, Debug, Odmation);
}
// #include <ImageHlp.h>
// #pragma comment(lib,"imagehlp.lib")
//#include <fstream>
//void game(CString path) {
//	std::ifstream file(path,std::ios::binary);
//	file.seekg(0,std::ios::end);
//	unsigned size = file.tellg();
//	char* filesize = new char[size];
//	file.seekg(0,std::ios::beg);
//	file.read(filesize, size);
//	file.close();
//	IMAGE_DOS_HEADER* dosheader = (IMAGE_DOS_HEADER*)filesize;
//	unsigned ped = dosheader->e_lfanew + (unsigned)filesize;
//	IMAGE_NT_HEADERS* ntheadr = (IMAGE_NT_HEADERS*)ped;
//	DWORD dent = ntheadr->OptionalHeader.AddressOfEntryPoint;
//	CString Wtext;
//	Wtext.Format(L"%X", dent);
//	AfxMessageBox(Wtext);
//	delete[] filesize;
//}

//右键项
void base1::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	index = pNMItemActivate->iItem;
	if (pNMItemActivate->iItem != -1) {
		DWORD dwpos = GetMessagePos();
		CPoint point(LOWORD(dwpos), HIWORD(dwpos));
		CMenu menu;
		menu.LoadMenuW(IDR_MENU1);
		CMenu* pop = menu.GetSubMenu(0);
		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

//删除一个项
void base1::deleteItem()
{
	// TODO: 在此添加命令处理程序代码
// 	CString val;
// 	val.Format(L"DATA_%d", index);

	wchar_t flilepath[100];
	GetCurrentDirectoryW(100, flilepath);
	CString strPath{ flilepath };
	strPath += "\\GameData.ini";

	listCtrl.DeleteItem(index);

	wchar_t buf[10]{};
	GetPrivateProfileStringW(L"NUMBER", L"LINK", NULL, buf, 10, strPath);
	int value = atoi((char*)buf);//总数

	CString num;
	for (int i = 0; i < value; i++)
	{
		CString gameName, gamePath, dllFile, curPathName, gameCommandParameter;
		gameName = listCtrl.GetItemText(i, 0);
		curPathName = listCtrl.GetItemText(i, 1);
		gamePath = listCtrl.GetItemText(i, 2);
		gameCommandParameter = listCtrl.GetItemText(i, 3);
		dllFile = listCtrl.GetItemText(i, 4);

		num.Format(L"DATA_%d", i);
		WritePrivateProfileStringW(num, L"LINK", L"gameName", gameName);
		WritePrivateProfileStringW(num, L"LINK", L"gameCommandParameter", gameCommandParameter);
		WritePrivateProfileStringW(num, L"LINK", L"dllFile", dllFile);
		WritePrivateProfileStringW(num, L"LINK", L"gamePath", gamePath);
		WritePrivateProfileStringW(num, L"LINK", L"curPathName", curPathName);
	}
	num.Format(L"DATA_%d", value - 1);
	::WritePrivateProfileSection(num, NULL, strPath);
	num.Format(L"%d", --value);
	WritePrivateProfileStringW(L"NUMBER", L"LINK", num, strPath);
	initWindow();//刷新屏幕
}

void base1::setItem()
{
	// TODO: 在此添加命令处理程序代码
	addGameDlg.BL = true;
	addGameDlg.index = index;
	OnBnClickedBtn();
	addGameDlg.BL = false;
}

void base1::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
}