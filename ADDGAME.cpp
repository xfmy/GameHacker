// ADDGAME.cpp: 实现文件
//

#include "pch.h"
#include "MFCGameHacker.h"
#include "ADDGAME.h"
#include "afxdialogex.h"

// ADDGAME 对话框

IMPLEMENT_DYNAMIC(ADDGAME, CDialogEx)

ADDGAME::ADDGAME(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDGAME_DLG, pParent)
	, gameName(_T(""))
	, gamePath(_T(""))
	, dllFile(_T(""))
	, curPathName(_T(""))
	, gameCommandParameter(_T(""))
{
}

ADDGAME::~ADDGAME()
{
}

void ADDGAME::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, gameName);
	DDX_Text(pDX, IDC_EDIT2, gamePath);
	DDX_Text(pDX, IDC_EDIT5, dllFile);
	DDX_Text(pDX, IDC_EDIT3, curPathName);
	DDX_Text(pDX, IDC_EDIT4, gameCommandParameter);
}

BEGIN_MESSAGE_MAP(ADDGAME, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ADDGAME::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ADDGAME::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &ADDGAME::OnBnClickedButton3)
END_MESSAGE_MAP()

// ADDGAME 消息处理程序

void ADDGAME::OnBnClickedButton1()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szpath[] = _T(" exe (*.exe)|*.exe|dat (*.dat)|*.dat|AllFiles (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, 6, szpath);
	if (fileDlg.DoModal() == IDOK)
	{
		//AfxMessageBox(fileDlg.GetPathName());
		gameName = fileDlg.GetFileTitle();
		gamePath = fileDlg.GetFolderPath() + L"\\";
		curPathName = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void ADDGAME::OnBnClickedButton2()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szpath[] = _T(" dll (*.dll)|*.dll|AllFiles (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, 6, szpath);
	if (fileDlg.DoModal() == IDOK)
	{
		dllFile = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void ADDGAME::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//检测游戏或者dll文件是否选择
	int a = gameName.GetLength();
	int b = dllFile.GetLength();
	int c = gameName.GetLength() && dllFile.GetLength();
	if (!gameName.GetLength() || !dllFile.GetLength())
	{
		AfxMessageBox(L"游戏或dll文件未选择");
		return;
	}

	//获取路径
	CHAR strPath[MAX_PATH]{};
	GetCurrentDirectoryA(MAX_PATH, strPath);//获取当前路径，存放在strpath中，
	strcat_s(strPath, MAX_PATH, "\\GameData.ini");

	//获取ini数量
	char strbuf[MAX_PATH]{};
	GetPrivateProfileStringA("NUMBER", "LINK", NULL, strbuf, MAX_PATH, strPath);
	int val = atoi(strbuf);
	//***********************************************************
	if (BL == true)
	{
		val = index;
	}
	//***********************************************************
	//写入数据
	CString str;
	str.Format(L"DATA_%d", val);
	CString cpath{ strPath };
	WritePrivateProfileStringW(str, L"gameName", gameName, cpath);
	WritePrivateProfileStringW(str, L"curPathName", curPathName, cpath);
	WritePrivateProfileStringW(str, L"gamePath", gamePath, cpath);
	WritePrivateProfileStringW(str, L"gameCommandParameter", gameCommandParameter, cpath);
	WritePrivateProfileStringW(str, L"dllFile", dllFile, cpath);

	//将数量写入ini文件
	if (BL == false) {
		str.Format(L"%d", ++val);
		WritePrivateProfileStringW(L"NUMBER", L"LINK", str, cpath);
	}
	CDialogEx::OnCancel();
}