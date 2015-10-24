
// MFCDialogDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "MFCDialog.h"
#include "MFCDialogDlg.h"
#include "QRCodeDlg.h"
#include "afxdialogex.h"
#include "Connection.h"
#include "AccountInfo.h"
#include "sqlite3.h" 
#include "resource.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�����ֽ�char*ת��Ϊ���ֽ�wchar_t*
wchar_t* AnsiToUnicode( const char* szStr )
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
	return pResult;
}

//�����ֽ�wchar_t*ת��Ϊ���ֽ�ANSI
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

//�����ֽ�wchar_t*ת��Ϊ���ֽ�UTF8
inline char* UnicodeToUtf8(const WCHAR *zWideFilename)
{
	int nByte; char *zFilename;
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = (char *)malloc(nByte);
	if (zFilename == 0)
	{
		return 0;
	}
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte, 0, 0);
	if (nByte == 0)
	{
		free(zFilename);
		zFilename = 0;
	}
	return zFilename;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strAboutQQ;
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
, m_strAboutQQ(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	m_strAboutQQ = _T("����Ⱥ��309397056");
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_strAboutQQ);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CAboutDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CAboutDlg::OnNMClickSyslink3)
END_MESSAGE_MAP()


// CMFCDialogDlg �Ի���



CMFCDialogDlg::CMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDialogDlg::IDD, pParent)
	//, m_UserName(_T(""))
	, m_password(_T(""))
	, m_cbString(_T(""))
	, m_bSavePw(FALSE)
	, m_bAutoCon(FALSE)
{
	m_StDialog=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCDialogDlg::~CMFCDialogDlg()
{
	if (hMObject != NULL)
	{
		CloseHandle(hMObject);
	}
	if (m_StDialog != NULL)
	{
		delete m_StDialog;
	}
}

void CMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, CtlComboBox);
	//	DDX_Text(pDX, IDC_EDIT2, m_UserName);
	DDX_Text(pDX, IDC_EDIT3, m_password);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_CBString(pDX, IDC_COMBO1, m_cbString);
	DDX_Check(pDX, IDC_CHECK1, m_bSavePw);
	DDX_Check(pDX, IDC_CHECK3, m_bAutoCon);
	DDX_Control(pDX, IDC_CHECK1, m_ctlSavePw);
	DDX_Control(pDX, IDC_CHECK3, m_ctlAutoCon);
}

BEGIN_MESSAGE_MAP(CMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMFCDialogDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCDialogDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCDialogDlg::OnBnClickedCheck3)
//	ON_BN_CLICKED(IDC_CHECK2, &CMFCDialogDlg::OnBnClickedCheck2)
//	ON_EN_CHANGE(IDC_EDIT2, &CMFCDialogDlg::OnEnChangeEdit2)
ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCDialogDlg::OnCbnSelchangeCombo1)
ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CMFCDialogDlg::OnNMClickSyslink2)
//ON_BN_CLICKED(IDC_CHECK1, &CMFCDialogDlg::OnBnClickedCheck1)
ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CMFCDialogDlg::OnNMClickSyslink3)
//ON_WM_RBUTTONUP()
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_32771, &CMFCDialogDlg::OnAboutMe)
ON_COMMAND(ID_32777, &CMFCDialogDlg::OnReadme)
ON_COMMAND(ID_32773, &CMFCDialogDlg::OnQRCode)
END_MESSAGE_MAP()


// CMFCDialogDlg ��Ϣ�������

BOOL CMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//����͸��
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 235, LWA_ALPHA);

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	hMObject = CreateMutex(NULL, FALSE, _T("CON_APP_XQ_2"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMObject);
		AfxMessageBox(_T("�ó�������һ��ʵ���������У�"));
		exit(FALSE);
	}

	/*BOOL bEnRet;
	SHELLEXECUTEINFO *lpSHELLEXECUTEINFO;
	lpSHELLEXECUTEINFO->cbSize=sizeof(SHELLEXECUTEINFO);
	lpSHELLEXECUTEINFO->lpVerb=_T("open");
	lpSHELLEXECUTEINFO->lpFile=_T("ConnectionInit");
	lpSHELLEXECUTEINFO->lpParameters=_T("-a");
	lpSHELLEXECUTEINFO->nShow=SW_SHOW;
	::ShellExecuteEx(lpSHELLEXECUTEINFO);*/
	DWORD dwEnRet;
	
	dwEnRet=RasValidateEntryName(NULL,_T("XQ_KLZF"));
	if(dwEnRet!=ERROR_ALREADY_EXISTS)
	{
		MessageBox(_T("RasEntry�����ڣ����򼴽���ʼ�����絯���û��˻����ƣ��������ǡ�"));
		TCHAR szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);
		(_tcsrchr(szPath, '\\'))[1] = 0;
		HINSTANCE hRet;
		hRet=ShellExecute(NULL,_T("open"),_T("ConnectionInit.exe"),_T("-a"),szPath,SW_HIDE);
		if((int)hRet<31)
		{
			CString err;
			err.Format(_T("��ʼ��RasEntryʧ�ܣ�������룺%d ����������װĿ¼���Install.bat������װ�����"),hRet);
			AfxMessageBox(err);
			exit(-2);
		}
	}
	
    CtlComboBox.SetCurSel(0); 

	int rc;  
	int i, nrows, ncols;  
	char *errmsg = NULL;  
	char **results;  
	sqlite3 *db = NULL;  
	char *dbPath;
	CString strDir(AnsiToUnicode(getenv("UserProfile")));
	strDir=strDir+_T("\\con_klzf");
	CreateDirectory(strDir,NULL);
	strDir = strDir + _T("\\demo.db");
	dbPath = UnicodeToUtf8(strDir);
	rc = sqlite3_open(dbPath, &db);  
	if (rc)  
	{  
		AfxMessageBox(AnsiToUnicode(sqlite3_errmsg(db)));
		sqlite3_close(db);  
		exit(1);  
	}  
	i=sqlite3_key(db,"pw",2);  //��Դ��ȫ���⣬��ʵ��������ȥ
	char **exsit;
	sqlite3_get_table(db,"SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Account_and_Setting';",&exsit,NULL,NULL,NULL);
	if(exsit[1][0]=='0')
	{
		if(SQLITE_OK!=sqlite3_exec(db,"CREATE TABLE Account_and_Setting (UserName VARCHAR(256) PRIMARY KEY,PW VARCHAR(256),LineSel INT,AutoConnect BOOLEAN,logtime TIMESTAMP DEFAULT (datetime('now', 'localtime') ) );",NULL,NULL,&errmsg))
		{
			CString strErr;
			strErr.Format(_T("��ʼ��ʧ�ܣ��������ڷ�����״̬��%s !"),errmsg);
			AfxMessageBox(strErr);
		}
	}
	sqlite3_free_table(exsit);
	sqlite3_get_table(db,"select UserName from Account_and_Setting order by logtime desc;",&results,&nrows,&ncols,&errmsg);
	if(nrows>=1)
	{
		for(i=1;i<=nrows;i++) 
		{  
			m_ComboBox.AddString(AnsiToUnicode(results[i]));   
		}  
		char sql_s[256];
		int nrows_2, ncols_2;  
		char *errmsg_2 = NULL;  
		char **results_2;  
		CString strUM;
		m_ComboBox.GetLBText(0,strUM);
		sprintf(sql_s,"select PW,LineSel,AutoConnect from Account_and_Setting where UserName='%s'",UnicodeToAnsi(strUM));
		sqlite3_get_table(db,sql_s,&results_2,&nrows_2,&ncols_2,&errmsg_2);
		if(nrows_2>=1)
		{
			m_password=AnsiToUnicode(results_2[3]);
			CtlComboBox.SetCurSel(results_2[4][0]-'0');
			m_bAutoCon=results_2[5][0]-'0';
			if(results_2[3]!=NULL)
				m_bSavePw=1;
			UpdateData(FALSE);
		}
		m_ComboBox.SetCurSel(0);
		sqlite3_free_table(results_2); //free
	}
	sqlite3_free_table(results); //free  
	sqlite3_close(db);  
	if(m_bAutoCon==1)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		this->PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON1,BN_CLICKED),NULL);
	}
	
	SwitchToThisWindow(this->m_hWnd,TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDialogDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

Connection con;
AccountInfo AccInfo;


void CMFCDialogDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	DWORD ret=0;     //MRet;
	//MFCDialogSt StDialog;
	AccInfo.setUserName(m_cbString);
	iCurSel=CtlComboBox.GetCurSel();
	//MRet=StDialog.DoModal();

	int rc;  
	int i;    
	sqlite3 *db = NULL;  
	char *dbPath;
	CString strDir(getenv("UserProfile"));
	strDir = strDir + _T("\\con_klzf\\demo.db");
	dbPath = UnicodeToUtf8(strDir);
	rc = sqlite3_open(dbPath, &db);  
	if (rc)  
	{  
		AfxMessageBox(AnsiToUnicode(sqlite3_errmsg(db)));
		sqlite3_close(db);  
		exit(1);  
	}  
	i=sqlite3_key(db,"pw",2);  //��Դ��ȫ���⣬��ʵ��������ȥ
	char sql[256];
	if(m_bSavePw)
		sprintf(sql,"replace into Account_and_Setting(UserName,PW,LineSel,AutoConnect) values('%s','%s',%d,%d)"
		,UnicodeToAnsi(m_cbString)
		,UnicodeToAnsi(m_password)
		,iCurSel
		,m_bAutoCon);
	else
		sprintf(sql,"replace into Account_and_Setting(UserName,PW,LineSel,AutoConnect) values('%s',NULL,%d,%d)"
		,UnicodeToAnsi(m_cbString)
		,iCurSel
		,m_bAutoCon);
	//AfxMessageBox(AnsiToUnicode(sql));
	sqlite3_exec(db,sql,0,0,0);
	sqlite3_close(db);  

	//AfxMessageBox(AccInfo.convertAccount(AccInfo.encryptAccount(),CtlComboBox.GetCurSel()));
	//AfxMessageBox(m_password);
	
	if(m_StDialog==NULL)
	{
		m_StDialog=new MFCDialogSt(this,m_cbString,m_password,iCurSel);
		m_StDialog->Create(IDD_STA_DIALOG,this);

	}
	m_StDialog->TaskBarTray(TRUE);
	//Shell_NotifyIcon(NIM_ADD, &(m_StDialog->NotifyIcon)); // �����������ͼ��
	this->ShowWindow(SW_HIDE);
	m_StDialog->ShowWindow(SW_SHOW);
	ret=con.Dial(L"XQ_KLZF",AccInfo.convertAccount(AccInfo.encryptAccount(),CtlComboBox.GetCurSel()),m_password,-1,m_StDialog->m_hWnd);
	if(ret!=0)
	{
		con.DisDial(NULL);
		CString strErr;
		TCHAR ErrorString[BUFFER_SIZE];
		DWORD dwRetVal;
		dwRetVal=RasGetErrorString(ret,ErrorString,BUFFER_SIZE);
		if(dwRetVal==ERROR_SUCCESS)
		{
			strErr.Format(_T("��������ʧ�ܣ�������� %d ��%s ��Ͽ������ӻ����������������"),ret,ErrorString);
		}
		else
		{
			strErr.Format(_T("��������ʧ�ܣ�������� %d δ֪�Ĵ�����Ϣ��"),ret);
		}
		m_StDialog->TaskBarTray(FALSE);
		//Shell_NotifyIcon(NIM_DELETE, &(m_StDialog->NotifyIcon));   // ��������ɾ��ͼ��
		m_StDialog->ShowWindow(SW_NORMAL);
		m_StDialog->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		SwitchToThisWindow(this->m_hWnd,TRUE);
		this->ShowWindow(SW_SHOW);
		MessageBox(strErr,_T("У԰���ͻ��� By ����"),MB_OK|MB_ICONERROR);
		//m_StDialog->ShowWindow(SW_SHOW);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CMFCDialogDlg::PreTranslateMessage(MSG* pMsg) 
{
 if(pMsg->message == WM_KEYDOWN) 
 {
  if(pMsg->wParam == VK_RETURN)//�����¼�Ϊ�س���ʱ
  {
   OnBnClickedButton1();//���ð�ť����
   return TRUE;
  }
 }
 return CDialog::PreTranslateMessage(pMsg);
}

void CMFCDialogDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ctlAutoCon.GetCheck())
		m_ctlSavePw.SetCheck(1);
}


/*void CMFCDialogDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}*/


//void CMFCDialogDlg::OnEnChangeEdit2()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CMFCDialogDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strUM;
	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(),strUM);
	int rc;  
	int i,nrows, ncols;  
	char *errmsg = NULL;  
	char **results;  
	char sql[256];
	sqlite3 *db = NULL;  
	char *dbPath;
	CString strDir(getenv("UserProfile"));
	strDir = strDir + _T("\\con_klzf\\demo.db");
	dbPath = UnicodeToUtf8(strDir);
	rc = sqlite3_open(dbPath, &db);  
	if (rc)  
	{  
		AfxMessageBox(AnsiToUnicode(sqlite3_errmsg(db)));
		sqlite3_close(db);  
		exit(1);  
	}  
	i=sqlite3_key(db,"pw",2);  //��Դ��ȫ���⣬��ʵ��������ȥ
	sprintf(sql,"select PW,LineSel,AutoConnect from Account_and_Setting where UserName='%s'",UnicodeToAnsi(strUM));
	sqlite3_get_table(db,sql,&results,&nrows,&ncols,&errmsg);
	if(nrows>=1)
	{
		m_password=AnsiToUnicode(results[3]);
		CtlComboBox.SetCurSel(results[4][0]-'0');
		m_bAutoCon=results[5][0]-'0';
		m_bSavePw=0;
		if(results[3]!=NULL)
			m_bSavePw=1;
		UpdateData(FALSE);
	}
	sqlite3_free_table(results); //free  
	sqlite3_close(db);  
}


void CMFCDialogDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strUM;
	int index=m_ComboBox.GetCurSel();
	//m_ComboBox.GetLBText(index,strUM);
	int rc;  
	int i;  
	char *errmsg = NULL;   
	char sql[256];
	sqlite3 *db = NULL;  
	char *dbPath;
	CString strDir(getenv("UserProfile"));
	strDir = strDir + _T("\\con_klzf\\demo.db");
	dbPath = UnicodeToUtf8(strDir);
	rc = sqlite3_open(dbPath, &db);  
	if (rc)  
	{  
		AfxMessageBox(AnsiToUnicode(sqlite3_errmsg(db)));
		sqlite3_close(db);  
		exit(1);  
	}  
	i=sqlite3_key(db,"pw",2);  //��Դ��ȫ���⣬��ʵ��������ȥ
	sprintf(sql,"delete from Account_and_Setting where UserName='%s';",UnicodeToAnsi(m_cbString));
	if(SQLITE_OK==sqlite3_exec(db,sql,0,0,&errmsg))
	{
		m_ComboBox.DeleteString(index);
		CtlComboBox.SetCurSel(0);
		//GetDlgItem(IDC_COMBO1)->SetWindowText(NULL);
		m_cbString.Empty();
		m_password.Empty();
		m_bAutoCon=0;
		m_bSavePw=0;
		UpdateData(FALSE);
		AfxMessageBox(_T("ɾ���ɹ���"));
	}
	else
	{
		CString strError;
		strError.Format(_T("ɾ��ʧ�ܣ�%s"),errmsg);
		AfxMessageBox(strError);
	}
	sqlite3_close(db);  
	*pResult = 0;
}


//void CMFCDialogDlg::OnBnClickedCheck1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CMFCDialogDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HKEY hRoot = HKEY_CURRENT_USER;
	TCHAR *szSubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	TCHAR szModule[MAX_PATH];
	
	GetModuleFileName(NULL, szModule, MAX_PATH);      //��ȡ����ľ���·��
	
	HKEY hKey;
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	DWORD dwType = REG_SZ;
	LONG lRet = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
	{
		CString strErr;
		strErr.Format(_T("����ע���ʧ��!������룺%d !"),lRet);
		AfxMessageBox(strErr);
	}
	else
	{ 
		TCHAR szData[MAX_PATH+2];
		DWORD dwSize; 
		if(ERROR_SUCCESS==RegQueryValueEx(hKey,_T("Connection"),0,&dwType,(LPBYTE)szData,&dwSize)&&0==_tcscmp(szData,szModule))
		{
			lRet = RegDeleteValue(hKey,_T("Connection"));
			RegCloseKey(hKey);
			if(lRet != ERROR_SUCCESS)
			{
				CString strErr;
				strErr.Format(_T("ע���д��ʧ��!δ��ȡ��������������������룺%d !"),lRet);
				AfxMessageBox(strErr);
			}
			else
				AfxMessageBox(_T("������������ȡ��"));
		}
		else
		{
			lRet = RegSetValueEx(hKey,_T("Connection"),0,REG_SZ,(LPBYTE)szModule,sizeof(TCHAR)*_tcslen(szModule));
			RegCloseKey(hKey);
			if(lRet != ERROR_SUCCESS)
			{
				CString strErr;
				strErr.Format(_T("ע���д��ʧ��!δ�����ÿ�����������������룺%d !"),lRet);
				AfxMessageBox(strErr);
			}
			else
				AfxMessageBox(_T("�������������óɹ���"));
		}
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	if (_tcscmp(pNMLink->item.szUrl, _T("http://www.klzf.tk")) == 0)
	{
		//ִ��
		ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	//AfxMessageBox(pNMLink->item.szUrl);
	if (_tcscmp(pNMLink->item.szUrl, _T("http://www.xiongqi.tk")) == 0)
	{
		//ִ��
		ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;

	QRCodeDlg QRCode;  
	nRes = QRCode.DoModal();
	if (IDOK == nRes)
		return;
	*pResult = 0;
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//����͸��
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 220, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


//void CMFCDialogDlg::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CMenu   menu;   //��������Ҫ�õ���cmenu����
//	menu.LoadMenu(IDR_MENU_ABOUT); //װ���Զ�����Ҽ��˵� 
//	CMenu   *pContextMenu = menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
//	CPoint point1;//����һ������ȷ�����λ�õ�λ��  
//	GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
//	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //��ָ��λ����ʾ�����˵�
//
//	CDialogEx::OnRButtonUp(nFlags, point);
//}


void CMFCDialogDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO:  �ڴ˴������Ϣ����������
	CMenu   menu;   //��������Ҫ�õ���cmenu����
	menu.LoadMenu(IDR_MENU_ABOUT); //װ���Զ�����Ҽ��˵� 
	CMenu   *pContextMenu = menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	CPoint point1;//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //��ָ��λ����ʾ�����˵�
}


void CMFCDialogDlg::OnAboutMe()
{
	// TODO:  �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMFCDialogDlg::OnReadme()
{
	// TODO:  �ڴ���������������
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	(_tcsrchr(szPath, '\\'))[1] = 0;
	ShellExecute(NULL, _T("open"), _T("Readme.txt"), NULL, szPath, SW_SHOWNORMAL);
}


void CMFCDialogDlg::OnQRCode()
{
	// TODO:  �ڴ���������������
	INT_PTR nRes;

	QRCodeDlg QRCode;
	nRes = QRCode.DoModal();
	if (IDOK == nRes)
		return;
}
