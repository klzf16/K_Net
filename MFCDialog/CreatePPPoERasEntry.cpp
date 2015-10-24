//CreatePPPoeRasEntry.cpp  CreatePPPoeRasEntry类实现
#pragma warning(disable:4996)

#include"TCHAR.h"
#include"CreatePPPoeRasEntry.h"


TCHAR* CreatePoeRasEntry::GetEntryName(void)
{
	return EntryName;
}

void CreatePoeRasEntry::InitRASIP(RASIPADDR &rasIP)
{
	rasIP.a =0;
	rasIP.b =0;
	rasIP.c =0;
	rasIP.d =0;
}

DWORD CreatePoeRasEntry::CreateRasEntry(TCHAR strEntryName[])
{
	_tcscpy(EntryName,strEntryName);
	LPRASENTRY rasEntry  =  NULL;
	DWORD dwBufferSize  =   0 ;
	DWORD dwRet  =   0 ;

	RasGetEntryProperties(NULL, L"" , NULL,  &dwBufferSize, NULL, NULL); 
	if (dwBufferSize  ==   0 )
		return   -1;

	rasEntry  =  (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	if  (rasEntry  ==  NULL)
		return   -2;


	memset(rasEntry, 0, sizeof( RASENTRYA ) );
	rasEntry->dwSize = dwBufferSize;
	rasEntry->dwfOptions = RASEO_RequireEncryptedPw|RASEO_RemoteDefaultGateway;
	rasEntry->dwFrameSize = 0;
	rasEntry->dwfNetProtocols = RASNP_Ip;
	rasEntry->dwFramingProtocol = RASFP_Ppp;
	_tcscpy(rasEntry->szDeviceType,RASDT_PPPoE);
	_tcscpy(rasEntry->szDeviceName,_T("WAN 微型端口 (PPPOE)"));
	rasEntry->dwAlternateOffset = 0;
	rasEntry->dwCountryID = 86;
	rasEntry->dwCountryCode = 86;
	rasEntry->dwChannels = 0;
	rasEntry->dwReserved1 = 0;
	rasEntry->dwReserved2 = 0;
	rasEntry->dwType=RASET_Broadband;
	//rasEntry->dwfOptions=RASEO_PreviewUserPw|RASEO_RemoteDefaultGateway;   //显示用户界面


	rasEntry->dwfOptions=~RASEO_RequireEncryptedPw & rasEntry->dwfOptions;


	_tcscpy (rasEntry->szX25PadType, _T(""));
	_tcscpy (rasEntry->szX25Address, _T(""));
	_tcscpy (rasEntry->szX25Facilities, _T(""));
	_tcscpy (rasEntry->szX25UserData, _T(""));
	_tcscpy (rasEntry->szScript, _T(""));
	_tcscpy (rasEntry->szAutodialDll, _T(""));
	_tcscpy (rasEntry->szAutodialFunc, _T(""));
	_tcscpy (rasEntry->szAreaCode, _T(" "));
	_tcscpy (rasEntry->szLocalPhoneNumber, _T(""));
	InitRASIP (rasEntry->ipaddr);
	InitRASIP (rasEntry->ipaddrDns);
	InitRASIP (rasEntry->ipaddrDnsAlt);
	InitRASIP (rasEntry->ipaddrWins);
	InitRASIP (rasEntry->ipaddrWinsAlt);

	// The RasSetEntryProperties function changes the connection information 
	//for an entry in the phone book or creates a new phone-book entry.（reference MSDN）
	//  创建连接 返回值为0则创建成功
	dwRet  =  RasSetEntryProperties(NULL, strEntryName , rasEntry, dwBufferSize, NULL,  NULL);  
	HeapFree(GetProcessHeap(),  0 , (LPVOID)rasEntry);
	return dwRet;
}

DWORD CreatePoeRasEntry::DelRasEntry(TCHAR strEntryName[])       //删除连接，成功则返回0，删除前应断开连接！
{
	DWORD info;
	info=RasDeleteEntry(NULL,strEntryName);
	return info;
}