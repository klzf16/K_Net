
// MFCDialogDlg.cpp : 实现文件
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

//将单字节char*转化为宽字节wchar_t*
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

//将宽字节wchar_t*转化为单字节ANSI
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

//将宽字节wchar_t*转化为单字节UTF8
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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
	m_strAboutQQ = _T("交流群：309397056");
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_strAboutQQ);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CAboutDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CAboutDlg::OnNMClickSyslink3)
END_MESSAGE_MAP()


// CMFCDialogDlg 对话框



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


// CMFCDialogDlg 消息处理程序

BOOL CMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	//设置透明
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 235, LWA_ALPHA);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	hMObject = CreateMutex(NULL, FALSE, _T("CON_APP_XQ_2"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMObject);
		AfxMessageBox(_T("该程序已有一个实例正在运行！"));
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
		MessageBox(_T("RasEntry不存在，程序即将初始化，如弹出用户账户控制，请点击“是”"));
		TCHAR szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);
		(_tcsrchr(szPath, '\\'))[1] = 0;
		HINSTANCE hRet;
		hRet=ShellExecute(NULL,_T("open"),_T("ConnectionInit.exe"),_T("-a"),szPath,SW_HIDE);
		if((int)hRet<31)
		{
			CString err;
			err.Format(_T("初始化RasEntry失败，错误代码：%d 请进入软件安装目录点击Install.bat，或重装软件。"),hRet);
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
	i=sqlite3_key(db,"pw",2);  //因开源安全问题，真实密码已隐去
	char **exsit;
	sqlite3_get_table(db,"SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Account_and_Setting';",&exsit,NULL,NULL,NULL);
	if(exsit[1][0]=='0')
	{
		if(SQLITE_OK!=sqlite3_exec(db,"CREATE TABLE Account_and_Setting (UserName VARCHAR(256) PRIMARY KEY,PW VARCHAR(256),LineSel INT,AutoConnect BOOLEAN,logtime TIMESTAMP DEFAULT (datetime('now', 'localtime') ) );",NULL,NULL,&errmsg))
		{
			CString strErr;
			strErr.Format(_T("初始化失败，程序工作于非正常状态！%s !"),errmsg);
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDialogDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
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
	i=sqlite3_key(db,"pw",2);  //因开源安全问题，真实密码已隐去
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
	//Shell_NotifyIcon(NIM_ADD, &(m_StDialog->NotifyIcon)); // 在托盘区添加图标
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
			strErr.Format(_T("网络连接失败，错误代码 %d ！%s 请断开该连接或重新启动计算机！"),ret,ErrorString);
		}
		else
		{
			strErr.Format(_T("网络连接失败，错误代码 %d 未知的错误信息！"),ret);
		}
		m_StDialog->TaskBarTray(FALSE);
		//Shell_NotifyIcon(NIM_DELETE, &(m_StDialog->NotifyIcon));   // 在托盘区删除图标
		m_StDialog->ShowWindow(SW_NORMAL);
		m_StDialog->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		SwitchToThisWindow(this->m_hWnd,TRUE);
		this->ShowWindow(SW_SHOW);
		MessageBox(strErr,_T("校园网客户端 By 熊奇"),MB_OK|MB_ICONERROR);
		//m_StDialog->ShowWindow(SW_SHOW);
	}
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CMFCDialogDlg::PreTranslateMessage(MSG* pMsg) 
{
 if(pMsg->message == WM_KEYDOWN) 
 {
  if(pMsg->wParam == VK_RETURN)//当按下键为回车键时
  {
   OnBnClickedButton1();//调用按钮函数
   return TRUE;
  }
 }
 return CDialog::PreTranslateMessage(pMsg);
}

void CMFCDialogDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ctlAutoCon.GetCheck())
		m_ctlSavePw.SetCheck(1);
}


/*void CMFCDialogDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}*/


//void CMFCDialogDlg::OnEnChangeEdit2()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


void CMFCDialogDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	i=sqlite3_key(db,"pw",2);  //因开源安全问题，真实密码已隐去
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
	// TODO: 在此添加控件通知处理程序代码
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
	i=sqlite3_key(db,"pw",2);  //因开源安全问题，真实密码已隐去
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
		AfxMessageBox(_T("删除成功！"));
	}
	else
	{
		CString strError;
		strError.Format(_T("删除失败！%s"),errmsg);
		AfxMessageBox(strError);
	}
	sqlite3_close(db);  
	*pResult = 0;
}


//void CMFCDialogDlg::OnBnClickedCheck1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CMFCDialogDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	HKEY hRoot = HKEY_CURRENT_USER;
	TCHAR *szSubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	TCHAR szModule[MAX_PATH];
	
	GetModuleFileName(NULL, szModule, MAX_PATH);      //获取程序的绝对路径
	
	HKEY hKey;
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	DWORD dwType = REG_SZ;
	LONG lRet = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
	{
		CString strErr;
		strErr.Format(_T("查找注册表失败!错误代码：%d !"),lRet);
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
				strErr.Format(_T("注册表写入失败!未能取消开机自启动，错误代码：%d !"),lRet);
				AfxMessageBox(strErr);
			}
			else
				AfxMessageBox(_T("开机自启动已取消"));
		}
		else
		{
			lRet = RegSetValueEx(hKey,_T("Connection"),0,REG_SZ,(LPBYTE)szModule,sizeof(TCHAR)*_tcslen(szModule));
			RegCloseKey(hKey);
			if(lRet != ERROR_SUCCESS)
			{
				CString strErr;
				strErr.Format(_T("注册表写入失败!未能设置开机自启动，错误代码：%d !"),lRet);
				AfxMessageBox(strErr);
			}
			else
				AfxMessageBox(_T("开机自启动设置成功！"));
		}
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	if (_tcscmp(pNMLink->item.szUrl, _T("http://www.klzf.tk")) == 0)
	{
		//执行
		ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	//AfxMessageBox(pNMLink->item.szUrl);
	if (_tcscmp(pNMLink->item.szUrl, _T("http://www.xiongqi.tk")) == 0)
	{
		//执行
		ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
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

	// TODO:  在此添加额外的初始化

	//设置透明
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 220, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


//void CMFCDialogDlg::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	CMenu   menu;   //定义下面要用到的cmenu对象
//	menu.LoadMenu(IDR_MENU_ABOUT); //装载自定义的右键菜单 
//	CMenu   *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
//	CPoint point1;//定义一个用于确定光标位置的位置  
//	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
//	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //在指定位置显示弹出菜单
//
//	CDialogEx::OnRButtonUp(nFlags, point);
//}


void CMFCDialogDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO:  在此处添加消息处理程序代码
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU_ABOUT); //装载自定义的右键菜单 
	CMenu   *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	CPoint point1;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, AfxGetMainWnd()); //在指定位置显示弹出菜单
}


void CMFCDialogDlg::OnAboutMe()
{
	// TODO:  在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMFCDialogDlg::OnReadme()
{
	// TODO:  在此添加命令处理程序代码
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	(_tcsrchr(szPath, '\\'))[1] = 0;
	ShellExecute(NULL, _T("open"), _T("Readme.txt"), NULL, szPath, SW_SHOWNORMAL);
}


void CMFCDialogDlg::OnQRCode()
{
	// TODO:  在此添加命令处理程序代码
	INT_PTR nRes;

	QRCodeDlg QRCode;
	nRes = QRCode.DoModal();
	if (IDOK == nRes)
		return;
}
