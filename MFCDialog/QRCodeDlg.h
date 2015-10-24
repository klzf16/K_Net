#pragma once
#include "resource.h"
#include "afxwin.h"

// QRCodeDlg 对话框

class QRCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QRCodeDlg)

public:
	QRCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~QRCodeDlg();

// 对话框数据
	enum { IDD = IDD_QR_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_QQNum;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
