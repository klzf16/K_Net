
// MFCDialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include"MFCDialogSt.h"


// CMFCDialogDlg �Ի���
class CMFCDialogDlg : public CDialogEx
{

// ����
public:
	CMFCDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMFCDialogDlg();

	MFCDialogSt *m_StDialog;
	HANDLE hMObject;

// �Ի�������
	enum { IDD = IDD_MFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//void CALLBACK RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
