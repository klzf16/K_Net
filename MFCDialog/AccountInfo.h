//========================================================
// AccountInfo.h
//========================================================
#ifndef ACCOUNT_INFO_H
#define ACCOUNT_INFO_H

#include "stdafx.h"

#define CMCC 0
#define CHINAUNICOM 1
#define NORMAL 2
#define CHINANET 3
#define DIRECT 4
typedef char LINE;

class AccountInfo
{
private:
	//CString strPassword;     //����
	CString strEName;       //�����˺�
	CString strCName;        //ת�����˺�
public:
	CString strUserName;     //�˺�
	void setUserName(const CString m_strUserName);
	//void setPassword(const CString m_strPassword);
	CString encryptAccount(CString strUserName);           //�˺���Ϣ����
	CString encryptAccount();           //�˺���Ϣ����
	CString convertAccount(const CString strTName,const LINE cFlag);      //�˺���Ϣת��
};

#endif