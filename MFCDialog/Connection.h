//========================================================
// Connection.h   RasDial、RasHangUp、etc...
//========================================================

#ifndef CONNECTION_H
#define CONNECTION_H

#include "stdafx.h"
#include <windows.h>
#include <Ras.h>
#pragma comment(lib,"Rasapi32.lib")
#pragma comment(lib,"comctl32.lib")

class Connection
{
private:
	CString RasEntryName;            //连接入口名
	HRASCONN hRascon;                //连接句柄
	CString RasDialName;             //拨号用户名
public:
	RASCONNSTATUS RasConnStatus;     //拨号状态结构指针
	DWORD Dial(const CString strEntryName,const CString strDialName,const CString strPassword,DWORD FuncFlag,LPVOID func);
	DWORD DisDial(HRASCONN hRascon);   //断开连接,若断开Dial成员函数所创建的连接，可传入参数NULL
	HRASCONN EnumConnection();         //枚举连接，返回连接句柄
	CString GetEntryName();
	CString GetDialName();
	OSVERSIONINFO osvi;                //系统版本号
	DWORD GetStatusRet(HRASCONN hRascon);
};

#endif