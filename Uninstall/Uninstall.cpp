// Uninstall.cpp : 定义控制台应用程序的入口点。
//
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

#include "TCHAR.h"
#include "windows.h"

int main(int argc, TCHAR* argv[])
{
	//FreeConsole();
	int ret;
	ret=MessageBoxW(NULL,_T("是否保留用户配置文件？"),_T("卸载 校园网客户端 BY XQ"),MB_YESNO);
	if(ret==IDNO)
	{
		TCHAR szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);
		(_tcsrchr(szPath, '\\'))[1] = 0;
		system("@echo off & C: & cd  %userprofile% & rd /q /s con_klzf>nul 2>nul");
		ShellExecute(NULL,L"open",_T("ConnectionInit.exe"),_T("-u"),szPath,SW_HIDE);
	}
	ShellExecute(NULL,_T("open"),_T("msiexec.exe"),_T("/x {F512EDFB-E079-473E-A25B-F174C31D9E95}"),NULL,SW_NORMAL);
	return 0;
}

