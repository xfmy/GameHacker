// MFCGameHackerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCGameHacker.h"
#include "MFCGameHackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCGameHackerDlg 对话框

CMFCGameHackerDlg::CMFCGameHackerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCGAMEHACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

bool CMFCGameHackerDlg::addDialog(CDialogEx* obj, int IDC, CString name, bool isShow = false)
{
	if (link > MAX_DLG_NUM)
		return false;

	obj->Create(IDC, this);
	if (link == 0)	isShow = true;
	obj->ShowWindow(isShow);
	dlg[link] = obj;
	m_tab.InsertItem(link, name);

	CRect re;
	GetClientRect(&re);
	re.top += 45;
	re.left += 20;
	re.bottom -= 20;
	re.right -= 20;
	dlg[link]->MoveWindow(&re);

	link++;
	return true;
}

/*	// 游戏名称
	CString gameName;
	// 游戏路径
	CString gamePath;
	afx_msg void OnBnClickedButton2();
	CString dllFile;
	CString curPathName;
	CString gameCommandParameter;*/
void CMFCGameHackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMFCGameHackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCGameHackerDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMFCGameHackerDlg 消息处理程序

BOOL CMFCGameHackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	addDialog(&base_1, IDD_DIALOG1, L"注入");
	addDialog(&base_2, IDD_DIALOG2, L"注入检测");
	addDialog(&m_gameAnalyse, IDD_ANALYSE_DIALOG, L"游戏分析");

	m_gameAnalyse.init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCGameHackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCGameHackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGameHackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCGameHackerDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int cur = m_tab.GetCurSel();
	for (int i = 0; i < link; i++)
	{
		dlg[i]->ShowWindow(i == cur);
	}
}

void CMFCGameHackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;

	CRect re;
	GetClientRect(&re);
	//m_tab.MoveWindow(&re);
	//m_tab.GetClientRect(&re);
	re.top += 45;
	re.left += 20;
	re.bottom -= 20;
	re.right -= 20;

	for (int i = 0; i < link; i++)
	{
		if (dlg[i])
			dlg[i]->MoveWindow(&re);
	}
}