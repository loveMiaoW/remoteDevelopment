// CMyCmd.cpp: 实现文件
//

#include "pch.h"
#include "remoteControl_server.h"
#include "afxdialogex.h"
#include "CMyCmd.h"


// CMyCmd 对话框

IMPLEMENT_DYNAMIC(CMyCmd, CDialogEx)

CMyCmd::CMyCmd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMyCmd::~CMyCmd()
{
}

void CMyCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD, m_edit);
	DDX_Control(pDX, IDC_COM, m_com);
}


BEGIN_MESSAGE_MAP(CMyCmd, CDialogEx)
	ON_MESSAGE(WM_CMDDATA,&CMyCmd::onCmdData)
	ON_BN_CLICKED(IDC_BTN, &CMyCmd::OnBnClickedBtn)
END_MESSAGE_MAP()


// CMyCmd 消息处理程序


void CMyCmd::onCmd()
{
	// TODO: 在此添加命令处理程序代码

}


BOOL CMyCmd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pmessage->setHw(GetSafeHwnd());
	HEAD_INFO head;
	head.commend = OPEN_CMD;
	head.len = 0;
	int ret = m_pmessage->sendMess(m_pmessage->m_client_socket,(char*)&head,sizeof(head));
	if (ret == false)
	{
		AfxMessageBox("启动CMD失败\n");
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMyCmd::setMessage(Socket* _obj)
{
	m_pmessage = _obj;
}

LRESULT CMyCmd::onCmdData(WPARAM wparam, LPARAM lparam)
{
	char* data = (char*)wparam;
	if (strcmp(data,m_input.GetBuffer()) == 0)
	{
		delete[] data;
		return 0;
	}
	m_edit.ReplaceSel(data);
	delete[] data;
	return 0;
}


void CMyCmd::OnBnClickedBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}
