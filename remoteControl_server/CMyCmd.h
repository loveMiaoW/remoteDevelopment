#pragma once
#include "afxdialogex.h"
#include "Socket.h"
#include "info.h"
// CMyCmd 对话框

class CMyCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CMyCmd)

public:
	CMyCmd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyCmd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void onCmd();
	virtual BOOL OnInitDialog();
	void setMessage(Socket *_obj);
	afx_msg LRESULT onCmdData(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedBtn();
public:
	Socket* m_pmessage;
	CEdit m_edit;
	CEdit m_com;
	CString m_input;
	CString m_output;
	
};
