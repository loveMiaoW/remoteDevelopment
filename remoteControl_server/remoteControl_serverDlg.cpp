
// remoteControl_serverDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "remoteControl_server.h"
#include "remoteControl_serverDlg.h"
#include "afxdialogex.h"
#include "info.h"
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
public:
	afx_msg void onOpenCalc();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_32772, &CAboutDlg::onOpenCalc)
END_MESSAGE_MAP()


// CremoteControlserverDlg 对话框



CremoteControlserverDlg::CremoteControlserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTECONTROL_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CremoteControlserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CremoteControlserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_ACCEPTSUCCESS, &CremoteControlserverDlg::onAcceptSucc)
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CremoteControlserverDlg::OnNMRClickList1)
	ON_COMMAND(ID_32772, &CremoteControlserverDlg::onOpenNot)
	ON_COMMAND(ID_32773, &CremoteControlserverDlg::onCmd)
	ON_COMMAND(ID_32771, &CremoteControlserverDlg::onOpenCalc)
END_MESSAGE_MAP()


// CremoteControlserverDlg 消息处理程序

BOOL CremoteControlserverDlg::OnInitDialog()
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
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("IP地址"), LVCFMT_CENTER,300);
	m_list.InsertColumn(0, _T("端口"), LVCFMT_CENTER, 300);
	//m_list.InsertItem(0, _T("127.0.0.1"));
	//m_list.SetItemText(0, 1, _T("80"));
	m_message.setHw(GetSafeHwnd());
	m_message.startSock();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CremoteControlserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CremoteControlserverDlg::OnPaint()
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
HCURSOR CremoteControlserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CremoteControlserverDlg::onAcceptSucc(WPARAM wparam, LPARAM lparam)
{
	SOCKET cliendSo = (SOCKET)wparam;
	PSOCKADDR_IN si = (PSOCKADDR_IN)lparam;
	listInsertText(si);
	return 0;
}

void CremoteControlserverDlg::listInsertText(PSOCKADDR_IN _si)
{
	CString ip;
	CString port;
	ip.Format("%s",inet_ntoa(_si->sin_addr));
	port.Format("%d", _si->sin_port);
	m_list.InsertItem(0, ip);
	m_list.SetItemText(0,1,port);
}



void CremoteControlserverDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int n = pNMItemActivate->iItem;

	if (n != -1)
	{
		CMenu menu, * pop;
		menu.LoadMenu(IDR_MENU1);
		pop = menu.GetSubMenu(0);
		CPoint point;
		ClientToScreen(&point);
		GetCursorPos(&point);
		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
	}
	
	*pResult = 0;
}


void CAboutDlg::onOpenCalc()
{
	// TODO: 在此添加命令处理程序代码

}


void CremoteControlserverDlg::onOpenNot()
{
	// TODO: 在此添加命令处理程序代码
	HEAD_INFO head;
	head.commend = OPEN_NOT;
	head.len = 0;
	m_message.sendMess(m_message.m_client_socket,(char*)&head, sizeof(head));
}


void CremoteControlserverDlg::onCmd()
{
	// TODO: 在此添加命令处理程序代码
	CMyCmd process;
	process.setMessage(&m_message);
	process.DoModal();
}


void CremoteControlserverDlg::onOpenCalc()
{
	// TODO: 在此添加命令处理程序代码
	HEAD_INFO head;
	head.commend = OPEN_CALC;
	head.len = 0;
	m_message.sendMess(m_message.m_client_socket, (char*)&head, sizeof(head));
}
