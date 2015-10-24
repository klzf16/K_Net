//========================================================
// HeartbeatSocket.cpp  send and recv UDP data
//========================================================

#pragma once

#include "stdafx.h"
#include <ws2tcpip.h>
#include<WinSock2.h>
#include<Windows.h>

#pragma comment(lib,"ws2_32.lib")

volatile int HeartFlag=1;



int UDPHeartbeat(char szDestIp[],int iPort,int timeout,char szSendData[],int iLen)
{
	WSADATA wsaData;                                    //指向WinSocket信息结构的指针
	SOCKET sockListener;                                //创建套接字
	SOCKADDR_IN saUdpServ;                              //指向通信对象的结构体指针                          
	BOOL fBroadcast = TRUE;                             //用于setsockopt(),表示允许
	int flag1=0,flag2=0;                                          //返回值
	char szRecvData[57]={0};

	if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)      //进行WinSocket的初始化
	{
		MessageBox(NULL,L"ERROR!!Can't initiates windows socket!Program stop!",L"————校园网联网客户端  By 熊奇——",MB_ICONERROR); 
		return -1;                                      //初始化失败返回-1
	}

	//建立一个数据报类型的UDP套接字 
	//setsockopt函数用于设置套接口选项,采用广播形式须将第三个参数设置为SO_BROADCAST
	sockListener=socket(PF_INET,SOCK_DGRAM,0);	

	setsockopt(sockListener,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));	//设置发送超时
	setsockopt(sockListener,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(int));	//设置接收超时

	//参数设置，注意要将IP地址设为INADDR_BROADCAST，表示发送广播UDP数据报
	setsockopt(sockListener,SOL_SOCKET,SO_BROADCAST, (char *)&fBroadcast,sizeof(BOOL));


	saUdpServ.sin_family = AF_INET;
	saUdpServ.sin_addr.s_addr = inet_addr(szDestIp);	
	saUdpServ.sin_port = htons(iPort);					 //发送用的端口

	//htonl(INADDR_BROADCAST);

	flag1=0;
	flag2=0;

	//使用sendto函数进行通信
	flag1=sendto(sockListener,szSendData,iLen,0,(SOCKADDR*)&saUdpServ,sizeof(SOCKADDR_IN)); 		
	flag2=recv(sockListener,szRecvData,iLen,0);
	if(flag1!=flag2||flag2<=0)	
		return (flag1+flag2);

	closesocket(sockListener);                           //关闭监听socket
	WSACleanup();		                                 // 关闭socket
	return 0;
}
