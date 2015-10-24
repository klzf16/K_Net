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
	WSADATA wsaData;                                    //ָ��WinSocket��Ϣ�ṹ��ָ��
	SOCKET sockListener;                                //�����׽���
	SOCKADDR_IN saUdpServ;                              //ָ��ͨ�Ŷ���Ľṹ��ָ��                          
	BOOL fBroadcast = TRUE;                             //����setsockopt(),��ʾ����
	int flag1=0,flag2=0;                                          //����ֵ
	char szRecvData[57]={0};

	if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)      //����WinSocket�ĳ�ʼ��
	{
		MessageBox(NULL,L"ERROR!!Can't initiates windows socket!Program stop!",L"��������У԰�������ͻ���  By ���桪��",MB_ICONERROR); 
		return -1;                                      //��ʼ��ʧ�ܷ���-1
	}

	//����һ�����ݱ����͵�UDP�׽��� 
	//setsockopt�������������׽ӿ�ѡ��,���ù㲥��ʽ�뽫��������������ΪSO_BROADCAST
	sockListener=socket(PF_INET,SOCK_DGRAM,0);	

	setsockopt(sockListener,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));	//���÷��ͳ�ʱ
	setsockopt(sockListener,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(int));	//���ý��ճ�ʱ

	//�������ã�ע��Ҫ��IP��ַ��ΪINADDR_BROADCAST����ʾ���͹㲥UDP���ݱ�
	setsockopt(sockListener,SOL_SOCKET,SO_BROADCAST, (char *)&fBroadcast,sizeof(BOOL));


	saUdpServ.sin_family = AF_INET;
	saUdpServ.sin_addr.s_addr = inet_addr(szDestIp);	
	saUdpServ.sin_port = htons(iPort);					 //�����õĶ˿�

	//htonl(INADDR_BROADCAST);

	flag1=0;
	flag2=0;

	//ʹ��sendto��������ͨ��
	flag1=sendto(sockListener,szSendData,iLen,0,(SOCKADDR*)&saUdpServ,sizeof(SOCKADDR_IN)); 		
	flag2=recv(sockListener,szRecvData,iLen,0);
	if(flag1!=flag2||flag2<=0)	
		return (flag1+flag2);

	closesocket(sockListener);                           //�رռ���socket
	WSACleanup();		                                 // �ر�socket
	return 0;
}
