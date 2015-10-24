//========================================================
// CreatePPPoeRasEntry.h   CreatePPPoE_RasEntry
//========================================================

#ifndef CPOEENTRY_H
#define CPOEENTRY_H

#include "TCHAR.h"
#include <windows.h>
#include <Ras.h>
#pragma comment(lib,"Rasapi32.lib")
#pragma comment(lib,"comctl32.lib")

class CreatePoeRasEntry
{
private:
	TCHAR EntryName[_MAX_FNAME];
public:
	TCHAR* GetEntryName(void);
	void InitRASIP(RASIPADDR &rasIP);
	DWORD CreateRasEntry(TCHAR strEntryName[]);	
	DWORD DelRasEntry(TCHAR strEntryName[]);       //删除连接，成功则返回0，删除前应断开连接！
};

#endif