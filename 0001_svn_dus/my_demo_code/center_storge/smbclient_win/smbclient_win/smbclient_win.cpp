// smbclient_win.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <winsock.h>
#include <process.h>
#include <winsvc.h>
#include <shlobj.h>
#include <objbase.h>
#include <WinNetWk.h>//WNetGetConnection 使用
#include <atlbase.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mpr")//WNetGetConnection 使用
char* GetHostIP(char * hostname,char * IP);
DWORD ExploreFile(char* path);
int main(){
	printf("hello world\n");
	char address[128] = {0},address2[128] = {0},address3[128] = {0};
	char buf[10];
	unsigned long len;
	char IP[64];
	DWORD ret;
	int WSA_return;
	WSADATA WSAData;
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD( 2, 2 );
	WSA_return = WSAStartup(wVersionRequested,&WSAData);
	strcpy_s(IP,GetHostIP("192.168.40.172",NULL));
	printf("IP = %s\n",IP);
	strcpy_s(buf,"K:");
	len = 128;
	GetHostIP(IP,address2);
	printf("address2 = %s\n",address2);
	ret = WNetGetConnection(buf,address,&len);
	if (ret == NO_ERROR ){
		printf("ret = %d\n",ret);
		char *ps;
		printf("strlen(address) = %d %s\n",strlen(address),address);
		if (strlen(address) > 3 )
		if ( ( ps = strchr(address + 2,'\\')) ){
			*ps = 0;
			ps = address + 2;
			GetHostIP(ps,address3);
			if (strcmp(address3,address2) == 0){
				if (_access(buf,00) == 0){
				//if (access(buf,00) == 0){
					printf("if\n");
				}
				else
					printf("else\n");
			}
		}
	}else{
		printf("ret = %d\n",ret);
	}
	if(ret == ERROR_BAD_DEVICE){
		printf("ERROR_BAD_DEVICE\n");
	}else if(ret == ERROR_NOT_CONNECTED){
		printf("ERROR_NOT_CONNECTED\n");
	}else if(ret == ERROR_MORE_DATA){
		printf("ERROR_MORE_DATA\n");
	}else if(ret == ERROR_CONNECTION_UNAVAIL){
		printf("ERROR_CONNECTION_UNAVAIL\n");
	}else if(ret == ERROR_NO_NETWORK){
		printf("ERROR_NO_NETWORK\n");
	}else if(ret == ERROR_EXTENDED_ERROR){
		printf("ERROR_EXTENDED_ERROR\n");
	}else if(ret == ERROR_NO_NET_OR_BAD_PATH){
		printf("ERROR_NO_NET_OR_BAD_PATH\n");
	}
	ExploreFile("E:\\dus");
	system("pause");
	return 0;
}
char* GetHostIP(char* hostname,char*IP){
	struct hostent *hn;
	struct in_addr ia;
	static char sip[64];
	char* pResult;
	if (IP != NULL) pResult = IP;
	else{
		sip[0] = 0;
		pResult = sip;
	}
	*pResult = 0;
	if (hostname == NULL) return pResult;
	hn = gethostbyname(hostname);
	if (hn == NULL){
		strncpy_s(pResult,32,hostname,32);
		pResult[32] = 0;
		return pResult;
	}
	memcpy(&(ia.S_un.S_addr),hn->h_addr_list[0],hn->h_length);
	strcpy_s(pResult,32,inet_ntoa(ia));
	return pResult;
}
/*
 *使用explore打开文件目录 
 *path为目录的绝对路径如E:\\dus
 */
DWORD ExploreFile(char* path)
{

	char mwindir[512];
	char mcmd[1024];
    memset(mwindir,0,512);
    GetWindowsDirectory( mwindir,512);
	memset(mcmd,0,1024);
	
	sprintf(mcmd,"%s\\explorer.exe %s",mwindir,path);

   

	STARTUPINFO msi;
	PROCESS_INFORMATION mpi;

	memset(&msi,0,sizeof(msi));
	
	memset(&mpi,0,sizeof(mpi));
//	Sleep(1000);
	
	CreateProcess(NULL, mcmd, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &msi, &mpi);
	return mpi.dwProcessId;

}

