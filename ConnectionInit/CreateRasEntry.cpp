#include"CreatePPPoERasEntry.h"
#include<iostream>
using namespace std;

void UninstallRasEntry()
{
	int ret;
	CreatePoeRasEntry Entry;
	system("rasdial XQ_KLZF /disconnect>nul");
	ret=Entry.DelRasEntry(_T("XQ_KLZF"));
	if(ret!=0)
	{
		char szErr[256]={0};
		RasGetErrorStringA(ret,szErr,256);
		cout<<"������룺"<<ret<<szErr<<endl;
		cout<<"Ras�绰��ɾ��ʧ�ܣ�����õ绰���Ƿ���ڣ������������ʹ�ù���ԱȨ�����У�"<<endl;
		Sleep(3000);
	}
}

void InstallRasEntry()
{
	int ret;
	CreatePoeRasEntry Entry;
	ret=Entry.CreateRasEntry(_T("XQ_KLZF"));
	if(0!=ret)
	{
		char szErr[256]={0};
		RasGetErrorStringA(ret,szErr,256);
		cout<<"������룺"<<ret<<szErr<<endl;
		cout<<"Ras�绰������ʧ�ܣ���ʹ�ù���ԱȨ�����л������������"<<endl;
		Sleep(3000);
	}
}

void PrintError()
{
	cout<<"�����Ƿ�!���������� -a -u -?��ʽ�Ĳ�����"<<endl;
	cout<<"\nconnection [-a] [-u] [-?] \n"<<endl
					<<"����˵����\n"<<endl
					<<"-a\t����Ras���벾"<<endl
					<<"-u\tɾ��Ras���벾"<<endl
					<<"-?\t����\n"<<endl;
	system("pause");
}

int main(int argc,char **argv)
{
	if(argc!=2)
		PrintError();

	char arg;
	while(--argc>0&&(*++argv)[0]=='-')
	{
		while(arg=*++argv[0])
			switch(arg)
		{
			case 'a':
				InstallRasEntry();
				break;
			case 'u':
				UninstallRasEntry();
				break;
			case '?':
				cout<<"\nconnection [-a] [-u] [-?] \n"<<endl
					<<"����˵����\n"<<endl
					<<"-a\t����Ras���벾"<<endl
					<<"-u\tɾ��Ras���벾"<<endl
					<<"-?\t����\n"<<endl;
				break;
			default:
				cout<<endl<<"-"<<arg<<" ";
				PrintError();
				argc=0;
				exit(-1);
				break;
		}
	}
	if(argc)
		PrintError();
	return 0;
}
