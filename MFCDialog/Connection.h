//========================================================
// Connection.h   RasDial��RasHangUp��etc...
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
	CString RasEntryName;            //���������
	HRASCONN hRascon;                //���Ӿ��
	CString RasDialName;             //�����û���
public:
	RASCONNSTATUS RasConnStatus;     //����״̬�ṹָ��
	DWORD Dial(const CString strEntryName,const CString strDialName,const CString strPassword,DWORD FuncFlag,LPVOID func);
	DWORD DisDial(HRASCONN hRascon);   //�Ͽ�����,���Ͽ�Dial��Ա���������������ӣ��ɴ������NULL
	HRASCONN EnumConnection();         //ö�����ӣ��������Ӿ��
	CString GetEntryName();
	CString GetDialName();
	OSVERSIONINFO osvi;                //ϵͳ�汾��
	DWORD GetStatusRet(HRASCONN hRascon);
};

#endif