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
	//CString strPassword;     //密码
	CString strEName;       //加密账号
	CString strCName;        //转换后账号
public:
	CString strUserName;     //账号
	void setUserName(const CString m_strUserName);
	//void setPassword(const CString m_strPassword);
	CString encryptAccount(CString strUserName);           //账号信息加密
	CString encryptAccount();           //账号信息加密
	CString convertAccount(const CString strTName,const LINE cFlag);      //账号信息转换
};

#endif