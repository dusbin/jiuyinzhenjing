#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<malloc.h>
struct ret_type
{
        unsigned long start_code;
        unsigned long end_code;
};
int main(int argc,char *argv[])
{
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
	sprintf(strcmd,"mknod /dev/firstdriver0 c %s 0",buff);
	fp = popen(strcmd,"r");
	if(fp == NULL){
		return -1;
	}
	pclose(fp);
	fp = NULL;
        struct ret_type *tmp;
        tmp = (struct ret_type *)malloc(sizeof(struct ret_type));
        if(tmp == NULL)
        {
                printf("malloc error\n");
                return -1;
        }
        int fd,num;
        num = atoi(argv[1]);
        printf("num is %d\n",num);
 
        fd = open("/dev/firstdriver0",O_RDWR,S_IRUSR|S_IWUSR);
        if(fd != -1){
                int n;
                n = write(fd,&num,sizeof(int));
                if(n != sizeof(int)){
                        printf("write error\n");
                        goto  out;
                }
                n = read(fd,tmp,sizeof(*tmp));
                if(n != sizeof(*tmp)){
                        printf("read error, n =  %d\n",n);
                        goto out;
  }
                n = read(fd,tmp,sizeof(*tmp));
                if(n != sizeof(*tmp)){
                        printf("read error, n =  %d\n",n);
                        goto out;
                }
                printf("start_code is  %lu \n",tmp->start_code);
                printf("end_code is  %lu \n",tmp->end_code);
 
                close(fd);
        }
        else
                printf("device open failed \n");
 
        out:free(tmp);
        return 0;
}

