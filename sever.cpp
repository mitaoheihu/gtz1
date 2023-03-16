//#include "stdafx.h"
#include <stdio.h>
#include<iostream>
#include<winsock2.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define DEFAULT_PORT 5050
int main()
{
	int iPort=DEFAULT_PORT;
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData))
	{     cout<<"Winsock不能被初始化！";
	      WSACleanup();
	      return 0;
	}
	SOCKET sockSer, sockConn;
	sockSer=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSer,addrCli;
	addrSer.sin_family=AF_INET;
	addrSer.sin_port=htons(iPort);
	addrSer.sin_addr.s_addr=htonl(INADDR_ANY);

	//绑定套接字
	if(bind(sockSer,(SOCKADDR*)&addrSer,sizeof(SOCKADDR))==SOCKET_ERROR)
	{
		printf("bind() Failed: %d\n",WSAGetLastError());
		return 0;
	}
	listen(sockSer,5);
	
	int len=sizeof(SOCKADDR);
	cout<<"服务器等待客户端的连接......"<<endl;

	sockConn=accept(sockSer,(SOCKADDR*)&addrCli,&len);
	if(sockConn==INVALID_SOCKET)
	{
		cout<<"服务器接受客户端连接失败！"<<endl;
		return 0;
	}
	else      cout<<"服务器接受客户端连接成功！"<<endl;
	char sendbuf[256],recvbuf[256];

	while(1)
	{
		if(recv(sockConn,recvbuf,256,0)>0)
			cout<<"客户端说：>"<<recvbuf<<endl;
		else
		{
			cout<<"客户端已断开连接"<<endl;
			break;
		}
		cout<<"服务器说：>";
		cin>>sendbuf;
		if(strcmp(sendbuf,"bye")==0)
		{
			break;
		}
		send(sockConn,sendbuf,strlen(sendbuf)+1,0);
	}
	closesocket(sockSer);
	WSACleanup();
	return 0;
}
