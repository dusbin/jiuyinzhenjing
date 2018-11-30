/* File Name: client.c */    
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1){
        perror("socket"),exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);//连接端口
    addr.sin_addr.s_addr = inet_addr("192.168.40.151");//都是服务器的，改成连接IP
    int res = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if (res == -1){
        perror("connect"),exit(-1);
    }
    printf("连接成功\n");

    char buf[100] = {};
    while (1){
        memset(buf,0,sizeof(buf));//buf清0
        printf("请输入要说的话\n");
        scanf("%s",buf);
        //write(sockfd,buf,strlen(buf));
		send(sockfd,buf,strlen(buf),0);
        if (strcmp(buf,"bye") == 0){//退出的合适位置
            break;
        }
        memset(buf,0,sizeof(buf));//buf清0
        //res = read(sockfd,buf,sizeof(buf));
		res = recv(sockfd,buf,sizeof(buf),0);
        printf("读到了%d字节，内容：%s\n",res,buf);
    }

    close(sockfd);
    return 0;
}
