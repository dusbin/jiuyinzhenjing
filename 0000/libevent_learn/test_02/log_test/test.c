#include <stdio.h>
#include <time.h>
#include <event2/event.h>
#include <string.h>//sleep
#include <unistd.h>
#include <stdlib.h>
#include <event.h>
#include <errno.h>
#include "../log/inc/log.h"

void accept_cb(int fd,short events,void *arg);
void socket_read_cb(int fd,short events,void *arg);
int tcp_server_init(int port,int listen_num);

int main(){
	init_log("/opt/test.log");
	LOG(EVENT_LOG_ERR,"test");
	release_log();
	return 0;
}

/*--------------------- 
作者：luotuo44 
来源：CSDN 
原文：https://blog.csdn.net/luotuo44/article/details/39670221?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
 