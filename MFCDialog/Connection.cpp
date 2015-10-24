//Connection.cpp  Connection类实现
#pragma warning(disable:4996)
#include"stdafx.h"
#include"Connection.h"

DWORD Connection::Dial(CString strEntryName,CString strDialName,CString strPassword,DWORD FuncFlag,LPVOID func)
{

	RasEntryName=strEntryName;
	RasDialName=strDialName;
	RASDIALPARAMS rdParams;
	memset(&rdParams,'\0', sizeof(rdParams));
	//rdParams.dwSize=sizeof(RASDIALPARAMS);
	ZeroMemory(&osvi, sizeof(osvi));
	GetVersionEx(&osvi);

#if (WINVER >= 0x401)
	if ((osvi.dwMajorVersion < 4) ||
		((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion < 1)) )
	{
		rdParams.dwSize = offsetof(RASDIALPARAMSW, dwSubEntry);
	}
	else
#endif
#if (WINVER >= 0x601)
		if ((osvi.dwMajorVersion < 6) ||
			((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion < 1)) )
		{
			rdParams.dwSize = offsetof(RASDIALPARAMSW, dwIfIndex);
		}
		else
#endif
		{
			rdParams.dwSize = sizeof(rdParams);
		}
		_tcscpy(rdParams.szEntryName,RasEntryName); //拨号连接名称
		rdParams.szPhoneNumber[0]=_T('\0');
		rdParams.szCallbackNumber[0]=_T('\0');
		_tcscpy(rdParams.szUserName,RasDialName);
		_tcscpy(rdParams.szPassword,strPassword);
		rdParams.szDomain[0]=_T('\0');
		hRascon=NULL;

		DWORD info= RasDial(NULL,NULL,&rdParams,FuncFlag,func,&hRascon);

		if(info!=ERROR_SUCCESS)
		{
			DisDial(hRascon);
		}
		return info;
}

DWORD Connection::DisDial(HRASCONN hRascon)
{
	DWORD dwInfo;
	if(hRascon==NULL&&this->hRascon!=NULL)
	{
		dwInfo=RasHangUp(this->hRascon); 
		while(GetStatusRet(this->hRascon)!=ERROR_INVALID_HANDLE)
		{
			Sleep(1);
		}
	}
	else
	{
		dwInfo=RasHangUp(hRascon);
		while(GetStatusRet(hRascon)!=ERROR_INVALID_HANDLE)
		{
			Sleep(1);
		}
	}
	return dwInfo;
}

CString Connection::GetEntryName()
{
	return this->RasEntryName;
}

CString Connection::GetDialName()
{
	return this->RasDialName;
}

DWORD Connection::GetStatusRet(HRASCONN hRascon)
{
	memset(&RasConnStatus,'\0', sizeof(RASCONNSTATUS));
#if (WINVER >= 0x401)
	if ((osvi.dwMajorVersion < 4) ||
		((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion < 1)) )
	{
		RasConnStatus.dwSize = offsetof(RASCONNSTATUS,szPhoneNumber);
	}
	else
#endif
#if (WINVER >= 0x601)
		if ((osvi.dwMajorVersion < 6) ||
			((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion < 1)) )
		{
			RasConnStatus.dwSize = offsetof(RASCONNSTATUS,localEndPoint);
		}
		else
#endif
		{
			RasConnStatus.dwSize = sizeof(RASCONNSTATUS);
		}
		return RasGetConnectStatus(hRascon,&RasConnStatus);
}