// MFCDialogSt.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDialog.h"
#include "MFCDialogSt.h"
#include "afxdialogex.h"
#include "Connection.h"
#include "AccountInfo.h"
#include "MFCDialogDlg.h"

#define  BUFFER_SIZE 256
#define WM_USER_NOTIFYICON WM_USER+5
#define ID_SHOW_WIN  10008
#define WM_USER_CLOSE 10010

const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);

extern Connection con;
extern AccountInfo AccInfo;
extern volatile int HeartFlag;

extern int UDPHeartbeat(char szDestIp[],int iPort,int timeout,char szSendData[],int iLen);
inline char* UnicodeToAnsi( const wchar_t* szStr )
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}

// MFCDialogSt 对话框

IMPLEMENT_DYNAMIC(MFCDialogSt, CDialogEx)

	MFCDialogSt::MFCDialogSt(CWnd* pParent /*=NULL*/)
	: CDialogEx(MFCDialogSt::IDD, pParent)
{

}

MFCDialogSt::MFCDialogSt(CWnd* pParent,CString strUserName,CString strPassword,int iCurSel): CDialogEx(MFCDialogSt::IDD, pParent)
{
	this->strUserName=strUserName;
	this->strPassword=strPassword;
	this->iCurSel=iCurSel;
}

MFCDialogSt::~MFCDialogSt()
{
}

void MFCDialogSt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MFCDialogSt, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MFCDialogSt::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(WM_RASEVENT, OnRasDialEvent)
	ON_MESSAGE(WM_USER_NOTIFYICON,OnNotifyMsg)
	ON_COMMAND(ID_SHOW_WIN,OnShowMsg)
	ON_COMMAND(WM_USER_CLOSE,OnUserCloseMsg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &MFCDialogSt::OnBnClickedButton2)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// MFCDialogSt 消息处理程序


BOOL MFCDialogSt::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//设置透明
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 210, LWA_ALPHA);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, FALSE);
	SetDlgItemText(IDC_STATIC_STA,_T("正在连接……"));
	this->UpdateWindow();
	// 设置托盘图标
	/*NotifyIcon.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	NotifyIcon.hWnd = m_hWnd;
	NotifyIcon.uID = IDR_MAINFRAME;
	NotifyIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO | NIF_SHOWTIP;
	NotifyIcon.uCallbackMessage = WM_USER_NOTIFYICON; // 自定义托盘菜单消息
	NotifyIcon.hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(NotifyIcon.szTip, _T("校园网客户端"));
	wcscpy_s(NotifyIcon.szInfo,_T( "各位亲们，这里也有我的图标哦~" ));     
	wcscpy_s(NotifyIcon.szInfoTitle,_T( "校园网客户端" ));
	NotifyIcon.uVersion=NOTIFYICON_VERSION_4; //打开高版本    
	NotifyIcon.dwInfoFlags= NIIF_USER | NIIF_LARGE_ICON ;    //打开自定义及大图标提示    
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon); */

	ModifyStyleEx(WS_EX_TOOLWINDOW,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void MFCDialogSt::TaskBarTray(BOOL on_off)
{
	if (on_off == TRUE)
	{
		//设置托盘图标
		NotifyIcon.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		NotifyIcon.hWnd = m_hWnd;
		NotifyIcon.uID = IDR_MAINFRAME;
		NotifyIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO | NIF_SHOWTIP;
		NotifyIcon.uCallbackMessage = WM_USER_NOTIFYICON; // 自定义托盘菜单消息
		NotifyIcon.hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		wcscpy(NotifyIcon.szTip, _T("校园网客户端"));
		wcscpy_s(NotifyIcon.szInfo,_T( "各位亲们，我在这儿哦~" ));
		wcscpy_s(NotifyIcon.szInfoTitle,_T( "校园网客户端" ));
		NotifyIcon.uVersion=NOTIFYICON_VERSION_4; //打开高版本
		NotifyIcon.dwInfoFlags= NIIF_USER | NIIF_LARGE_ICON ;    //打开自定义及大图标提示
		Shell_NotifyIcon(NIM_ADD, &NotifyIcon);  //添加托盘图标
	}
	else
		Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);  //删除托盘图标
}

void MFCDialogSt::OnBnClickedButton1()
{
	KillTimer(108);
	con.DisDial(NULL);
	this->ShowWindow(SW_HIDE);
	m_pParentWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	m_pParentWnd->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void MFCDialogSt::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDOK==MessageBox(_T("退出程序将会使网络连接断开，是否继续？"),_T("校园网客户端"),MB_OKCANCEL|MB_ICONQUESTION))
	{
		con.DisDial(NULL);
		m_pParentWnd->SendMessage(WM_CLOSE);
	}
	CDialogEx::OnClose();
}



LRESULT MFCDialogSt::OnRasDialEvent(WPARAM wp, LPARAM lp) 
{
	RASCONNSTATE rasstate= (RASCONNSTATE)wp; 
	CString str;
	switch(rasstate)
	{
	case RASCS_OpenPort:
		str= _T("正在打开端口...");
		break;
	case RASCS_PortOpened:
		str= _T("端口已经打开");
		break;
	case RASCS_ConnectDevice:
		str= _T("正在连接设备");
		break;
	case RASCS_DeviceConnected:
		str= _T("设备已经连接");
		break;
	case RASCS_AllDevicesConnected:
		str= _T("所有设备已经连接上");
		break;
	case RASCS_Authenticate:
		str= _T("正在验证用户名和密码...");
		break;
	case RASCS_AuthNotify:
		str= _T("IDS_RASCS_7");
		break;
	case RASCS_AuthRetry:
		str= _T("再次验证用户名和密码...");
		break;
	case RASCS_AuthCallback:
		str= _T("回收验证");
		break;
	case RASCS_AuthChangePassword:
		str= _T("改变密码请求");
		break;
	case RASCS_AuthProject:
		str= _T("开始连接状态");
		break;
	case RASCS_AuthLinkSpeed:
		str= _T("计算连接速度...");
		break;
	case RASCS_AuthAck:
		str= _T("Authentication Acknowledged");
		break;
	case RASCS_ReAuthenticate:
		str= _T("Reauthenticatation Started");
		break;
	case RASCS_Authenticated:
		str= _T("验证通过");
		break;
	case RASCS_PrepareForCallback:
		str= _T("Preparation For Callback");
		break;
	case RASCS_WaitForModemReset:
		str= _T("等待设备重置...");
		break;
	case RASCS_WaitForCallback:
		str= _T("Waiting For Callback");
		break;
	case RASCS_Interactive:
		str= _T("Interactive");
		break;
	case RASCS_RetryAuthentication:
		str= _T("再次验证");
		break;
	case RASCS_CallbackSetByCaller:
		str= _T("Callback Set By Caller");
		break;
	case RASCS_PasswordExpired:
		str= _T("密码过期");
		break;
	case RASCS_Connected:
		str= _T("已经连接上");
		break;
	case RASCS_Disconnected:
		str= _T("已经断开连接");
		break;
	default:
		str= _T("连接错误");
		break;
	}
	this->SetDlgItemText(IDC_STATIC_STA,str);
	DWORD dwError=(DWORD)lp;
	if(rasstate==RASCS_Connected)
	{
		SwitchToThisWindow(this->m_hWnd,TRUE);
		this->SetDlgItemText(IDC_STATIC_STA,_T("已连接"));
		this->UpdateWindow();
		SetTimer(108,30000,NULL);
		UDPHeartbeat("172.16.7.12", 3338, 3000, UnicodeToAnsi(AccInfo.convertAccount(strUserName, this->iCurSel)), 56);
		Sleep(2000);
		//Shell_NotifyIcon(NIM_ADD, &NotifyIcon); 
		this->ShowWindow(SW_HIDE); // 隐藏主窗口
	}
	if(dwError!=0)
	{
		con.DisDial(NULL);
		CString strErr;
		TCHAR ErrorString[BUFFER_SIZE];
		DWORD dwRetVal;
		dwRetVal=RasGetErrorString(dwError,ErrorString,BUFFER_SIZE);
		if(dwRetVal==ERROR_SUCCESS)
		{
			strErr.Format(_T("网络连接失败，错误代码 %d ！%s"),dwError,ErrorString);
		}
		else
		{
			strErr.Format(_T("网络连接失败，错误代码 %d 未知的错误信息！"),dwError);
		}
		TaskBarTray(FALSE);       // 在托盘区删除图标
		this->ShowWindow(SW_NORMAL);
		this->ShowWindow(SW_HIDE);
		m_pParentWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		SwitchToThisWindow(m_pParentWnd->m_hWnd,TRUE);
		m_pParentWnd->ShowWindow(SW_SHOW);
		MessageBox(strErr,_T("校园网客户端 By 熊奇"),MB_OK|MB_ICONERROR);
	}
	return (LRESULT)0;
}



void MFCDialogSt::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int Wret;
	char *szSendData;
	szSendData=UnicodeToAnsi(AccInfo.convertAccount(strUserName,this->iCurSel));
	Wret=UDPHeartbeat("172.16.7.12",3338,3000,szSendData,56);
	if(Wret!=0)
	{
		KillTimer(108);
		con.DisDial(NULL);
		this->SetDlgItemText(IDC_STATIC_STA,_T("网络已断开，正在重连……"));
		m_pParentWnd->ShowWindow(SW_SHOW);
		this->m_pParentWnd->PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON1,BN_CLICKED),NULL);
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT MFCDialogSt::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 单击最小化按钮
	if (message == WM_SYSCOMMAND && 
		wParam == SC_MINIMIZE)
	{
		if(IDOK==MessageBox(_T("是否最小化窗口到托盘"),_T("校园网客户端 By 熊奇"),MB_ICONQUESTION|MB_OKCANCEL))
		{
			//this->ShowWindow(SW_MINIMIZE); // 隐藏主窗口 
			this->PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON2,BN_CLICKED),NULL);
		}
		else 
			return 0;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


LRESULT MFCDialogSt::OnNotifyMsg(WPARAM wParam,LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) // 判断图标ID是否相符
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: 
		{// 按鼠标右键
			CPoint point;
			GetCursorPos(&point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, ID_SHOW_WIN, _T("显示")); 
			menu.AppendMenu(MF_STRING, WM_USER_CLOSE, _T("退出"));
			SetForegroundWindow();  // 解决菜单失去焦点不消失的BUG
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			menu.DestroyMenu(); 
		}
		break;
	case WM_LBUTTONDOWN:   //单击鼠标
	case WM_LBUTTONDBLCLK: // 双击鼠标左键
		OnShowMsg();
		break;
	}
	return 0;
}

void MFCDialogSt::OnShowMsg()
{
	this->ShowWindow(SW_SHOW);
	UpdateWindow();
	PostMessage(WM_SYSCOMMAND,SC_RESTORE);
}


void MFCDialogSt::OnDestroy()
{
	CDialogEx::OnDestroy();
	con.DisDial(NULL);
	// TODO: 在此处添加消息处理程序代码
}

void MFCDialogSt::OnUserCloseMsg()
{
	if(IDOK==MessageBox(_T("退出程序将会使网络连接断开，是否继续？"),_T("校园网客户端"),MB_OKCANCEL|MB_ICONQUESTION))
	{
		con.DisDial(NULL);
		m_pParentWnd->SendMessage(WM_CLOSE);
	}
	CDialogEx::OnClose();
}

void MFCDialogSt::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
}


void MFCDialogSt::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO:  在此处添加消息处理程序代码
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU_ABOUT); //装载自定义的右键菜单 
	CMenu   *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	CPoint point1;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //在指定位置显示弹出菜单
}

