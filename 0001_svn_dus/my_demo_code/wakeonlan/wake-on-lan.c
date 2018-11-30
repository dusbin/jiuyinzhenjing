#include <stdio.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
static unsigned char SwitchChar(char chStr){
	if(chStr>='0'&&chStr<='9'){
		return (chStr-'0');
	}else if(chStr>='A'&&chStr<='F'){
		return (chStr-'A'+10);
	}else if(chStr>='a'&&chStr<='f'){
		return (chStr-'a'+10);
	}else{
		return 0;
	}
}
int mac_str_to_int(char input[],char* output){
	char *pchStr = strtok(input,":");
	int i = 0;
	while(NULL!=pchStr){
		output[i++] = (SwitchChar(*pchStr)<<4)|SwitchChar(*(pchStr+1));
		pchStr = strtok(NULL,":");
	}
	return 0;
}
int print_mac(char input[],char* output){
	printf("input mac is [%s]",input);
	printf("output mac is");
	int i = 0;
	for(i = 0;i<6;i++){
		printf("%x",output[i]);
		if(i<5){
			printf(":");
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
	//unsigned char mac[6]={0x00,0x10,0x20,0x30,0x40,0x50};//目的主机MAC地址，例如: 00:10:20:30:40:50
	unsigned char mac[6]={0};
	unsigned char packet[102];
	char ip[32] = {0};
	char str_mac[32] = {0};
	int sockfd, i,j, on = 1;
	memset(ip,0x00,sizeof(ip));
	memset(str_mac,0x00,sizeof(str_mac));
	printf("input MAC:");
	scanf("%s",str_mac);
	for(i = 0;str_mac[i]!='0';i++){
		if(str_mac[i]=='-'){
			str_mac[i]=':';
		}
	}
	printf("input IP:");
	scanf("%s",ip);
	mac_str_to_int(str_mac,mac);
	print_mac(str_mac,mac);
	struct sockaddr_in addr;
		
	//构建magic packet
	for(i=0;i<6;i++)            
		packet[i] = 0xFF;               
		for(i=1;i<17;i++)
			for(j=0;j<6;j++)
				packet[i*6+j] = mac[j];
	
	//UDP
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//广播
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,&on, sizeof(on));
	if(sockfd < 0)
		exit(0);                
	memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	memset(ip,0x00,sizeof(ip));
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("xxx.xxx.xxx.xxx");//广播地址
	sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&addr, sizeof(addr));
	close(sockfd);
	return 0;
}