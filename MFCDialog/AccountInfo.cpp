#include "stdafx.h"
#include"AccountInfo.h"

void AccountInfo::setUserName(const CString m_strUserName)
{
	this->strUserName=m_strUserName;
}

/*void AccountInfo::setPassword(const CString m_strPassword)
{
	this->strPassword=m_strPassword;
}*/

CString AccountInfo::encryptAccount(const CString strUserName)
{
	int i=0;
	strEName=strUserName;
    for(i=0;i<strUserName.GetLength();i++)
        if(strUserName.GetAt(i)>='4'&&strUserName.GetAt(i)<='7')
            strEName.SetAt(i,(strUserName[i]^12));
        else
            strEName.SetAt(i,strUserName[i]^4);
	return strEName;
}

CString AccountInfo::encryptAccount()
{
	int i=0;
	strEName=strUserName;
    for(i=0;i<strUserName.GetLength();i++)
        if(strUserName.GetAt(i)>='4'&&strUserName.GetAt(i)<='7')
            strEName.SetAt(i,(strUserName[i]^12));
        else
            strEName.SetAt(i,strUserName[i]^4);
	return strEName;
}

CString AccountInfo::convertAccount(const CString strTName,const LINE cFlag)
{
	switch(cFlag)
	{
	case CMCC:
		strCName=_T("{SRUN3}\r\n")+strTName+_T("@cmcc");
		break;
	case CHINAUNICOM:
		strCName=_T("{SRUN3}\r\n")+strTName+_T("@chinaunicom");
		break;
	case NORMAL:
		strCName=_T("{SRUN3}\r\n")+strTName;
		break;
	case CHINANET:
		strCName=_T("{SRUN3}\r\n")+strTName+_T("@chinanet");
		break;
	case DIRECT:
		strCName=strUserName;
		break;
	default:
		strCName=strUserName;
		break;
	}
	return strCName;
}