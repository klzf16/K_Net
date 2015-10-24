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
	DWORD DelRasEntry(TCHAR strEntryName[]);       //ɾ�����ӣ��ɹ��򷵻�0��ɾ��ǰӦ�Ͽ����ӣ�
};

#endif