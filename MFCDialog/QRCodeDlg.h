#pragma once
#include "resource.h"
#include "afxwin.h"

// QRCodeDlg �Ի���

class QRCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QRCodeDlg)

public:
	QRCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~QRCodeDlg();

// �Ի�������
	enum { IDD = IDD_QR_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_QQNum;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
