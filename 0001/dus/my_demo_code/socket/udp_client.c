#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
//https://www.cnblogs.com/leezheng/p/8030011.html
int main(){
	//创建socket对象
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	//创建网络通信对象
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1324);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	while(1){
		//printf("input a number:\n");
		char buf=0;
		//scanf("%hhd",&buf);
		buf=rand()%65;
		//printf("before sendto\n");
		sendto(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&addr,sizeof(addr));
		socklen_t len=sizeof(addr);
		//printf("before recvfrom\n");
		recvfrom(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&addr,&len);
		if(66 == buf){
			printf(" server success.\n");
		}else{
			printf(" server failed.\n");
		}
		sleep(1);
	}
	close(sockfd);
}

