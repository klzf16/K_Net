
// MFCDialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include"MFCDialogSt.h"


// CMFCDialogDlg 对话框
class CMFCDialogDlg : public CDialogEx
{

// 构造
public:
	CMFCDialogDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMFCDialogDlg();

	MFCDialogSt *m_StDialog;
	HANDLE hMObject;

// 对话框数据
	enum { IDD = IDD_MFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	//void CALLBACK RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox CtlComboBox;
	//CString m_UserName;
	CString m_password;
	int iCurSel;
	afx_msg void OnBnClickedButton1();
	LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp); 
	//static UINT __cdecl ThreadProc(LPVOID lParam);
	afx_msg void OnBnClickedCheck3();
//	afx_msg void OnBnClickedCheck2();
//	afx_msg void OnEnChangeEdit2();
	CComboBox m_ComboBox;
	CString m_cbString;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bSavePw;
	BOOL m_bAutoCon;
	CButton m_ctlSavePw;
	CButton m_ctlAutoCon;
//	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAboutMe();
	afx_msg void OnReadme();
	afx_msg void OnQRCode();
};
