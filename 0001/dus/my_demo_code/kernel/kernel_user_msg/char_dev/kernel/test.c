#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "memdev.h"
int main(void)
{
    int fd = 0;
    int cmd;
    int arg = 0;
    char Buf[4096];
	char buff[16] = {0};
	char strcmd[1024] = {0};
	FILE * fp = NULL; 
    fp = popen("cat /proc/devices |grep memdev|awk -F' ' '{printf$1}'","r");
	if(fp == NULL){
		return -1;
	}
	//memset(buff,0x00,sizeof(buff));
	fgets(buff,sizeof(buff),fp);
	pclose(fp);
	fp = NULL;
	//memset(strcmd,0x00,sizeof(strcmd));
	sprintf(strcmd,"mknod /dev/memdev0 c %s 0",buff);
	fp = popen(strcmd,"r");
	if(fp == NULL){
		return -1;
	}
	pclose(fp);
	fp = NULL;
    fd = open("/dev/memdev0",O_RDWR);
    if(fd < 0)
    {
        printf("Open Dev Mem Erro\n");
        return -1;
    }
    
    printf("call memdev_iocprint\n");
    cmd = MEMDEV_IOCPRINT;
    printf("userspace cmd is : %ld\n",cmd);
    if(ioctl(fd,cmd,&arg) < 0)
    {
        printf("call cmd MEMDEV_IOCPRINT fail\n");
        return -1;
    }
    printf("call MEMDEV_IOCGETDATA\n");
    cmd = MEMDEV_IOCGETDATA;
    if(ioctl(fd,cmd,&arg) < 0)
    {
        printf("call cmd MEMDEV_IOCGETDATA fail\n");
        return -1;
    }
    printf("in user space MEMDEV_IOCGETDATA get data is %d\n\n",arg);
    close(fd);
    return 0;
}

