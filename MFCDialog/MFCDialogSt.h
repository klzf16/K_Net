#pragma once


// MFCDialogSt 对话框

class MFCDialogSt : public CDialogEx
{
	DECLARE_DYNAMIC(MFCDialogSt)
	NOTIFYICONDATA NotifyIcon;

public:
	MFCDialogSt(CWnd* pParent = NULL);   // 标准构造函数
	MFCDialogSt(CWnd* pParent, CString strUserName, CString strPassword, int iCurSel);
	virtual ~MFCDialogSt();

// 对话框数据
	enum { IDD = IDD_STA_DIALOG };

	CString strUserName;
	CString strPassword;
	int iCurSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	void TaskBarTray(BOOL on_off);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp); 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNotifyMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowMsg();
	afx_msg void OnUserCloseMsg();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
