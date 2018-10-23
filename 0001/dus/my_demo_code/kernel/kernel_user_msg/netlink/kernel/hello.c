#include <linux/module.h>
#include <linux/init.h>  
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>
MODULE_LICENSE("GPL");
#define NETLINK_TEST 25
#define MAX_MSGSIZE 1024
#define KER_NAME "TEST_dus "
int pid;
int err;
struct sock *nl_sk = NULL;
int flag = 0;
char str[100];
int stringlength(char *s){
	int slen = 0;
	for(; *s; s++){
		slen++;
	}
   	return slen;
}
void send_netlink_data(char *message){
	struct sk_buff *skb_1;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(MAX_MSGSIZE);
	int slen = 0;
 
	skb_1 = alloc_skb(len,GFP_KERNEL);
	if(!skb_1){
		printk(KER_NAME "my_net_link:alloc_skb_1 error\n");
	}
	slen = stringlength(message);
	nlh = nlmsg_put(skb_1,0,0,0,MAX_MSGSIZE,0);
	NETLINK_CB(skb_1).pid = 0;
	NETLINK_CB(skb_1).dst_group = 0;
	memcpy(NLMSG_DATA(nlh),message,slen+1);
	printk(KER_NAME "my_net_link:send = %d, message '%s'.\n",slen,(char *)NLMSG_DATA(nlh));
	netlink_unicast(nl_sk,skb_1,pid,MSG_DONTWAIT);
}
void recv_netlink_data(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int i=0;
	skb = skb_get (__skb);
	if(skb->len >= NLMSG_SPACE(0)){
		nlh = nlmsg_hdr(skb);
		memcpy(str, NLMSG_DATA(nlh), sizeof(str));
		printk(KER_NAME "Message received:[%s]\n",str) ;
		i = simple_strtol(str,NULL,10);
		i = i*i;
		sprintf(str,"%d",i);
		pid = nlh->nlmsg_pid;
		send_netlink_data(str);
		flag = 1;
		kfree_skb(skb);
	}
}
static int hello_init(void){
	nl_sk = netlink_kernel_create(&init_net,NETLINK_TEST,1,recv_netlink_data,NULL,THIS_MODULE);
	printk(KER_NAME "hello world\n");	
	if(!nl_sk){
		printk(KER_NAME "create netlink socket error.\n");
		return 1;
	}
	return 0;
}
static void hello_exit(void)
{
	if(nl_sk != NULL){
		sock_release(nl_sk->sk_socket);
	}
	printk(KER_NAME "Goodbye world\n");
}
module_init(hello_init);
module_exit(hello_exit);
