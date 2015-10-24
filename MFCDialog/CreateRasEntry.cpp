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
		cout<<"错误代码："<<ret<<szErr<<endl;
		cout<<"Ras电话簿删除失败，请检查该电话簿是否存在，或重启计算机使用管理员权限运行！"<<endl;
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
		cout<<"错误代码："<<ret<<szErr<<endl;
		cout<<"Ras电话簿创建失败，请使用管理员权限运行或重启计算机！"<<endl;
		Sleep(3000);
	}
}

void PrintError()
{
	cout<<"参数非法!请输入类似 -a -u -?形式的参数！"<<endl;
	cout<<"\nconnection [-a] [-u] [-?] \n"<<endl
					<<"参数说明：\n"<<endl
					<<"-a\t创建Ras号码簿"<<endl
					<<"-u\t删除Ras号码簿"<<endl
					<<"-?\t帮助\n"<<endl;
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
					<<"参数说明：\n"<<endl
					<<"-a\t创建Ras号码簿"<<endl
					<<"-u\t删除Ras号码簿"<<endl
					<<"-?\t帮助\n"<<endl;
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
