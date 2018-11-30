/* file name:server.c*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include "switch.h"
int sockfd;
void fa(int signo){
    printf("服务器正在关闭\n");
    sleep(1);
    close(sockfd);
    exit(0);
}
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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int reuseaddr = 1;//解决地址已被占用问题
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
            &reuseaddr,sizeof(reuseaddr));

    int res = bind(sockfd,(struct sockaddr*)&addr,
            sizeof(addr));
    if (res == -1){
        perror("bind"),exit(-1);
    }
    printf("bind ok\n");
    listen(sockfd,100);//监听
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
        while (1){
            //res = read(fd,buf,sizeof(buf));
			res = recv(fd,buf,sizeof(buf),0);
            printf("接受了%d字节，内容:%s\n",res,buf);
            if (res <= 0){//包括0和-1
                break;
            }
            if (strcmp(buf,"bye") == 0){
                break;
            }
			switch_func(buf,buff);
            //write(fd,buf,strlen(buf));
			//sprintf(buff,"you are [%s],and you say:[%s]",from,buf);
			printf("buff = %s\n",buff);
			send(fd,buff,strlen(buff),0);
            memset(buf,0,sizeof(buf));
        }
        close(fd);
        exit(0);
        }
    close(fd);
    }
}
