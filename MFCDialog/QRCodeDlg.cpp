// QRCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Connection.h"
#include "QRCodeDlg.h"
#include "afxdialogex.h"


// QRCodeDlg 对话框

IMPLEMENT_DYNAMIC(QRCodeDlg, CDialogEx)

QRCodeDlg::QRCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(QRCodeDlg::IDD, pParent)
	, m_QQNum(_T(""))
{

}

QRCodeDlg::~QRCodeDlg()
{
}

void QRCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_QQNum);
}


BEGIN_MESSAGE_MAP(QRCodeDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// QRCodeDlg 消息处理程序


BOOL QRCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_QQNum = _T("群号：309397056");
	UpdateData(FALSE);

	/*SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));  //加载库文件
	if (hInst)
	{
		typedef BOOL(WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC func = NULL;	//函数指针
		//取得SetLayeredWindowAttributes函数指针 
		func = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		//使用SetLayeredWindowAttributes函数设定透明度
		if (func)func(this->GetSafeHwnd(), RGB(0, 0, 0), 200, 0x2);
		FreeLibrary(hInst);
	}*/

	/*
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 200, LWA_ALPHA);
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void QRCodeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));//使窗口可拖动
	CDialogEx::OnLButtonDown(nFlags, point);
}
