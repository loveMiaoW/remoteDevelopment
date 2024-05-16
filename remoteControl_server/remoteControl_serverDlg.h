
// remoteControl_serverDlg.h: 头文件
//

#pragma once
#include "Socket.h"
#include "CMyCmd.h"
// CremoteControlserverDlg 对话框
class CremoteControlserverDlg : public CDialogEx
{
// 构造
public:
	CremoteControlserverDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTECONTROL_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT onAcceptSucc(WPARAM wparam,LPARAM lparam);
	void listInsertText(PSOCKADDR_IN _si);
public:
	CListCtrl m_list;
	Socket m_message;
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void onOpenNot();
	afx_msg void onCmd();
	afx_msg void onOpenCalc();
};
