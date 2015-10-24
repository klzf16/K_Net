// MFCDialogSt.cpp : ʵ���ļ�
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

// MFCDialogSt �Ի���

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

// MFCDialogSt ��Ϣ�������


BOOL MFCDialogSt::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����͸��
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 210, LWA_ALPHA);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, FALSE);
	SetDlgItemText(IDC_STATIC_STA,_T("�������ӡ���"));
	this->UpdateWindow();
	// ��������ͼ��
	/*NotifyIcon.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	NotifyIcon.hWnd = m_hWnd;
	NotifyIcon.uID = IDR_MAINFRAME;
	NotifyIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO | NIF_SHOWTIP;
	NotifyIcon.uCallbackMessage = WM_USER_NOTIFYICON; // �Զ������̲˵���Ϣ
	NotifyIcon.hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(NotifyIcon.szTip, _T("У԰���ͻ���"));
	wcscpy_s(NotifyIcon.szInfo,_T( "��λ���ǣ�����Ҳ���ҵ�ͼ��Ŷ~" ));     
	wcscpy_s(NotifyIcon.szInfoTitle,_T( "У԰���ͻ���" ));
	NotifyIcon.uVersion=NOTIFYICON_VERSION_4; //�򿪸߰汾    
	NotifyIcon.dwInfoFlags= NIIF_USER | NIIF_LARGE_ICON ;    //���Զ��弰��ͼ����ʾ    
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon); */

	ModifyStyleEx(WS_EX_TOOLWINDOW,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void MFCDialogSt::TaskBarTray(BOOL on_off)
{
	if (on_off == TRUE)
	{
		//��������ͼ��
		NotifyIcon.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		NotifyIcon.hWnd = m_hWnd;
		NotifyIcon.uID = IDR_MAINFRAME;
		NotifyIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO | NIF_SHOWTIP;
		NotifyIcon.uCallbackMessage = WM_USER_NOTIFYICON; // �Զ������̲˵���Ϣ
		NotifyIcon.hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		wcscpy(NotifyIcon.szTip, _T("У԰���ͻ���"));
		wcscpy_s(NotifyIcon.szInfo,_T( "��λ���ǣ��������Ŷ~" ));
		wcscpy_s(NotifyIcon.szInfoTitle,_T( "У԰���ͻ���" ));
		NotifyIcon.uVersion=NOTIFYICON_VERSION_4; //�򿪸߰汾
		NotifyIcon.dwInfoFlags= NIIF_USER | NIIF_LARGE_ICON ;    //���Զ��弰��ͼ����ʾ
		Shell_NotifyIcon(NIM_ADD, &NotifyIcon);  //�������ͼ��
	}
	else
		Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);  //ɾ������ͼ��
}

void MFCDialogSt::OnBnClickedButton1()
{
	KillTimer(108);
	con.DisDial(NULL);
	this->ShowWindow(SW_HIDE);
	m_pParentWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	m_pParentWnd->ShowWindow(SW_SHOW);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void MFCDialogSt::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(IDOK==MessageBox(_T("�˳����򽫻�ʹ�������ӶϿ����Ƿ������"),_T("У԰���ͻ���"),MB_OKCANCEL|MB_ICONQUESTION))
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
		str= _T("���ڴ򿪶˿�...");
		break;
	case RASCS_PortOpened:
		str= _T("�˿��Ѿ���");
		break;
	case RASCS_ConnectDevice:
		str= _T("���������豸");
		break;
	case RASCS_DeviceConnected:
		str= _T("�豸�Ѿ�����");
		break;
	case RASCS_AllDevicesConnected:
		str= _T("�����豸�Ѿ�������");
		break;
	case RASCS_Authenticate:
		str= _T("������֤�û���������...");
		break;
	case RASCS_AuthNotify:
		str= _T("IDS_RASCS_7");
		break;
	case RASCS_AuthRetry:
		str= _T("�ٴ���֤�û���������...");
		break;
	case RASCS_AuthCallback:
		str= _T("������֤");
		break;
	case RASCS_AuthChangePassword:
		str= _T("�ı���������");
		break;
	case RASCS_AuthProject:
		str= _T("��ʼ����״̬");
		break;
	case RASCS_AuthLinkSpeed:
		str= _T("���������ٶ�...");
		break;
	case RASCS_AuthAck:
		str= _T("Authentication Acknowledged");
		break;
	case RASCS_ReAuthenticate:
		str= _T("Reauthenticatation Started");
		break;
	case RASCS_Authenticated:
		str= _T("��֤ͨ��");
		break;
	case RASCS_PrepareForCallback:
		str= _T("Preparation For Callback");
		break;
	case RASCS_WaitForModemReset:
		str= _T("�ȴ��豸����...");
		break;
	case RASCS_WaitForCallback:
		str= _T("Waiting For Callback");
		break;
	case RASCS_Interactive:
		str= _T("Interactive");
		break;
	case RASCS_RetryAuthentication:
		str= _T("�ٴ���֤");
		break;
	case RASCS_CallbackSetByCaller:
		str= _T("Callback Set By Caller");
		break;
	case RASCS_PasswordExpired:
		str= _T("�������");
		break;
	case RASCS_Connected:
		str= _T("�Ѿ�������");
		break;
	case RASCS_Disconnected:
		str= _T("�Ѿ��Ͽ�����");
		break;
	default:
		str= _T("���Ӵ���");
		break;
	}
	this->SetDlgItemText(IDC_STATIC_STA,str);
	DWORD dwError=(DWORD)lp;
	if(rasstate==RASCS_Connected)
	{
		SwitchToThisWindow(this->m_hWnd,TRUE);
		this->SetDlgItemText(IDC_STATIC_STA,_T("������"));
		this->UpdateWindow();
		SetTimer(108,30000,NULL);
		UDPHeartbeat("172.16.7.12", 3338, 3000, UnicodeToAnsi(AccInfo.convertAccount(strUserName, this->iCurSel)), 56);
		Sleep(2000);
		//Shell_NotifyIcon(NIM_ADD, &NotifyIcon); 
		this->ShowWindow(SW_HIDE); // ����������
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
			strErr.Format(_T("��������ʧ�ܣ�������� %d ��%s"),dwError,ErrorString);
		}
		else
		{
			strErr.Format(_T("��������ʧ�ܣ�������� %d δ֪�Ĵ�����Ϣ��"),dwError);
		}
		TaskBarTray(FALSE);       // ��������ɾ��ͼ��
		this->ShowWindow(SW_NORMAL);
		this->ShowWindow(SW_HIDE);
		m_pParentWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		SwitchToThisWindow(m_pParentWnd->m_hWnd,TRUE);
		m_pParentWnd->ShowWindow(SW_SHOW);
		MessageBox(strErr,_T("У԰���ͻ��� By ����"),MB_OK|MB_ICONERROR);
	}
	return (LRESULT)0;
}



void MFCDialogSt::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int Wret;
	char *szSendData;
	szSendData=UnicodeToAnsi(AccInfo.convertAccount(strUserName,this->iCurSel));
	Wret=UDPHeartbeat("172.16.7.12",3338,3000,szSendData,56);
	if(Wret!=0)
	{
		KillTimer(108);
		con.DisDial(NULL);
		this->SetDlgItemText(IDC_STATIC_STA,_T("�����ѶϿ���������������"));
		m_pParentWnd->ShowWindow(SW_SHOW);
		this->m_pParentWnd->PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON1,BN_CLICKED),NULL);
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT MFCDialogSt::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	// ������С����ť
	if (message == WM_SYSCOMMAND && 
		wParam == SC_MINIMIZE)
	{
		if(IDOK==MessageBox(_T("�Ƿ���С�����ڵ�����"),_T("У԰���ͻ��� By ����"),MB_ICONQUESTION|MB_OKCANCEL))
		{
			//this->ShowWindow(SW_MINIMIZE); // ���������� 
			this->PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON2,BN_CLICKED),NULL);
		}
		else 
			return 0;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


LRESULT MFCDialogSt::OnNotifyMsg(WPARAM wParam,LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) // �ж�ͼ��ID�Ƿ����
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: 
		{// ������Ҽ�
			CPoint point;
			GetCursorPos(&point);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, ID_SHOW_WIN, _T("��ʾ")); 
			menu.AppendMenu(MF_STRING, WM_USER_CLOSE, _T("�˳�"));
			SetForegroundWindow();  // ����˵�ʧȥ���㲻��ʧ��BUG
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			menu.DestroyMenu(); 
		}
		break;
	case WM_LBUTTONDOWN:   //�������
	case WM_LBUTTONDBLCLK: // ˫��������
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
	// TODO: �ڴ˴������Ϣ����������
}

void MFCDialogSt::OnUserCloseMsg()
{
	if(IDOK==MessageBox(_T("�˳����򽫻�ʹ�������ӶϿ����Ƿ������"),_T("У԰���ͻ���"),MB_OKCANCEL|MB_ICONQUESTION))
	{
		con.DisDial(NULL);
		m_pParentWnd->SendMessage(WM_CLOSE);
	}
	CDialogEx::OnClose();
}

void MFCDialogSt::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
}


void MFCDialogSt::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO:  �ڴ˴������Ϣ����������
	CMenu   menu;   //��������Ҫ�õ���cmenu����
	menu.LoadMenu(IDR_MENU_ABOUT); //װ���Զ�����Ҽ��˵� 
	CMenu   *pContextMenu = menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	CPoint point1;//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //��ָ��λ����ʾ�����˵�
}

