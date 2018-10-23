#ifndef __SMBCLIENT_WIN_H__
#define __SMBCLIENT_WIN_H__
class CSysCommLayer{
public:
	static char* GetHostIP(char*hostname,char *ip=NULL);
}
#endif