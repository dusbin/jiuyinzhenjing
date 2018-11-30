#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>
#define NETLINK_TEST 25
#define MAX_PAYLOAD 1024 // maximum payload size
int sock_fd;
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
struct msghdr msg;
void init_netlink(){
	// Create a socket
	char input[64] = {0};
	sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
	if(sock_fd == -1){
		printf("error getting socket: %s", strerror(errno));
		return;
	}
	// To prepare binding
	memset(&msg,0,sizeof(msg));
	memset(&src_addr, 0, sizeof(src_addr));
	//src_address
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid(); // self pid
	src_addr.nl_groups = 0; // multi cast
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
 
	//dest_address
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0; /* For Linux Kernel */
	dest_addr.nl_groups = 0; /* unicast */

	/* Fill the netlink message header */
	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid(); /* self pid */
	nlh->nlmsg_flags = 0;

	/* Fill in the netlink message payload */
	printf("input:\n");
	scanf("%s",input);
	printf("you input [%s]\n",input);
	strcpy(NLMSG_DATA(nlh), input);
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	//iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
}
 
int main(int argc, char* argv[])
{
	int state;
	int state_smg = 0;	
	
	printf(" Waiting message. ...\n");
	// Read message from kernel
 
	while(1){
		state_smg = 0;
		init_netlink();
		printf(" Sending message. ...\n");
		state_smg = sendmsg(sock_fd,&msg,0);
		if(state_smg == -1){
			printf("get error sendmsg = %s\n",strerror(errno));
		}
		memset(nlh,0,NLMSG_SPACE(MAX_PAYLOAD));
		printf("In while recvmsg\n");
		state = recvmsg(sock_fd, &msg, 0);
		if(state<0){
			printf("state<1");
		}
		printf("In while\n");
		printf("Received message: [%s]\n",(char *) NLMSG_DATA(nlh));
		close(sock_fd);
	}
	
	return 0;
}