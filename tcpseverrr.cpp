#include "stdafx.h"
#include "resource.h"
#include<WINSOCK2.h>
#pragma comment(lib,"ws2_32.lib")
HINSTANCE hInst;								
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

SOCKET sockSer,sockConn;                  //服务器端创建两个套接字
SOCKADDR_IN addrSer,addrCli;
int iIndex=0; int len=sizeof(SOCKADDR);
char sendbuf[256],recvbuf[256];
char clibuf[999]="客户端:>",serbuf[999]="服务器:>";

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, NULL, (DLGPROC)About);			  
   return 0;
}
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char ip[16],port[5];
	switch (message)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_IP,"192.168.1.103");
			SetDlgItemText(hDlg,IDC_PORT,"5353");
			WSADATA wsaData;
			if(WSAStartup(MAKEWORD(2,2),&wsaData)){
				MessageBox(hDlg,"Winsock加载失败","警告",0);
				WSACleanup();
				return -1;
			}
			return TRUE;

		case WM_COMMAND:{
			switch(LOWORD(wParam))
			{case IDC_QUIT:
			EndDialog(hDlg, LOWORD(wParam));
			closesocket(sockSer);
			WSACleanup();
			return TRUE;
			case IDC_CREATE:
				sockSer=socket(AF_INET,SOCK_STREAM,0);
				GetDlgItemText(hDlg,IDC_IP,ip,16);
				GetDlgItemText(hDlg,IDC_PORT,port,5);
				EnableWindow(GetDlgItem(hDlg,IDC_CREATE),FALSE);
				//
				addrSer.sin_family=AF_INET;
				addrSer.sin_port=htons(atoi(port));
				addrSer.sin_addr.S_un.S_addr=inet_addr(ip);
				bind(sockSer,(SOCKADDR*)&addrSer,len);//
				listen(sockSer,5);
				sockConn=accept(sockSer,(SOCKADDR*)&addrCli,&len);
				if(sockConn!=INVALID_SOCKET)
					SendDlgItemMessage(hDlg,IDC_RECVBUF,LB_ADDSTRING,0,
					(LPARAM)"客户端已经连接上");
				break;
			case IDC_SEND:
				GetDlgItemText(hDlg,IDC_SENDBUF,sendbuf,256);
				send(sockConn,sendbuf,strlen(sendbuf)+1,0);
				SetDlgItemText(hDlg,IDC_SENDBUF,"");
				strcat(serbuf,sendbuf);
				//
				SendDlgItemMessage(hDlg,IDC_RECVBUF,LB_ADDSTRING,0,(LPARAM)serbuf);
				//
				//
				strcpy(serbuf,"服务器:>");
				break;

			case IDC_RECV:
				recv(sockConn,recvbuf,256,0);
				strcat(clibuf,recvbuf);
				SendDlgItemMessage(hDlg,IDC_RECVBUF,LB_ADDSTRING,0,(LPARAM)clibuf);
				strcpy(clibuf,"客户端:>");
				break;
			}
						}
	}
    return FALSE;
}
