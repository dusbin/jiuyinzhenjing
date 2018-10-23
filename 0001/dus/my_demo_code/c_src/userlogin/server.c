/* file name:server.c*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include <shadow.h>
#include <errno.h>
int sockfd;
void fa(int signo){
    printf("服务器正在关闭\n");
    sleep(1);
    close(sockfd);
    exit(0);
}
void get_salt(char *salt,char *passwd);
int procIsInPasswd(char* userName);
int ispass(int argc,char **argv);
int main(){
    printf("ctrl+c退出服务器\n");
    signal(SIGINT,fa);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1){
        perror("socket"),exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr("192.168.1.75");

    int reuseaddr = 1;//解决地址已被占用问题
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
            &reuseaddr,sizeof(reuseaddr));

    int res = bind(sockfd,(struct sockaddr*)&addr,
            sizeof(addr));
    if (res == -1){
        perror("bind"),exit(-1);
    }
	struct spwd *sp;
	char salt[512]={0};
    printf("bind ok\n");
    listen(sockfd,100);//监听
	char user[64] = {0};
	char passwd[128] = {0};
	char cmd[1024] = {0};
	char *pass = NULL;
    while (1){
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int fd = accept(sockfd,(struct sockaddr*)&client,
                &len);//阻塞函数
        char *from = inet_ntoa(client.sin_addr);
        printf("%s连接成功\n",from);
        pid_t pid = fork();
        if (pid == 0){
        char buf[100] = {};
		char buff[256] = {0};
		char *p = NULL;
        while (1){
            //res = read(fd,buf,sizeof(buf));
			res = recv(fd,buf,sizeof(buf),0);
            printf("接受了%d字节，内容:%s\n",res,buf);
			p = strtok(buf,":");
			memset(user,0x00,sizeof(user));
			strcpy(user,p);
			p = strtok(NULL,":");
			memset(passwd,0x00,sizeof(passwd));
			strcpy(passwd,p);
			printf("user is %s,passwd is %s\n",user,passwd);
			if(0 != procIsInPasswd(user)){
				// user is not exist
				strcpy(buf,"user is not exist");
			}else{
				if(0 == ispass(user,passwd)){
					strcpy(buf,"pass");
				}else{
					strcpy(buf,"no pass");
				}
			}
            if (res <= 0){//包括0和-1
                break;
            }
            if (strcmp(buf,"bye") == 0){
                break;
            }
            //write(fd,buf,strlen(buf));
			//sprintf(buff,"you are [%s],and you say:[%s]",from,buf);
			printf("buf = %s\n",buf);
			send(fd,buf,strlen(buf),0);
            memset(buf,0,sizeof(buf));
        }
        close(fd);
        exit(0);
        }
    close(fd);
    }
}
int ispass(int argc,char **argv){
	printf("1\n");
	struct spwd *sp;
    char salt[512]={0};
	char *passwd = argv[2];
    if(sp=getspnam(argv[1]) == NULL)
        return -1;
	printf("2 %s\n",sp->sp_pwdp);
    get_salt(salt,sp->sp_pwdp);
	printf("salt = %s\n",salt);
	char *pass = crypt(passwd,salt);
    if(strcmp(sp->sp_pwdp,pass) == 0)
        return 0;
    else
        return -1;
}
int procIsInPasswd(char* userName)
{

	int ret = -1;
	FILE *fp;

	char buf[1024] = {0};
	char name[128] = {0};
	char *p = NULL;
	char *q = NULL;

	if(NULL == userName)
		return -1;

	fp = fopen("/etc/passwd", "r");
	if(NULL == fp)
		return -1;
	while(fgets(buf, 1024, fp) > 0)
	{
		memset(name, 0x00, sizeof(name));
		p = buf;
		q = strchr(buf, ':');
		if(q != NULL)
		{
			memcpy(name, buf, q-p);
			if(strcmp(userName, name) == 0)
			{
				ret = 0;
				break;
			}
		}
		memset(buf, 0x00, sizeof(buf));
	}
	fclose(fp);
	return ret;

}
void get_salt(char *salt,char *passwd)
{
    int i,j;


    //取出salt,i记录密码字符下标,j记录$出现次数
    for(i=0,j=0;passwd[i] && j != 3;++i)
    {
		printf("aaa\n");
        if(passwd[i] == '$')
            ++j;
    }


    strncpy(salt,passwd,i-1);
}

