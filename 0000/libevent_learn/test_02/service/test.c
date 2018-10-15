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
	init_log("/opt/server.log");
	int listener = tcp_server_init(9999,10);
	if(listener == -1){
		LOG(EVENT_LOG_ERR,"tcp_server_init error(%d)",listener);
		return -1;
	}
	struct event_base* base = event_base_new();
	struct event* ev_listen = event_new(base,listener,EV_READ|EV_PERSIST,accept_cb,base);
	event_add(ev_listen,NULL);
	event_base_dispatch(base);
	release_log();
	return 0;
}
void accept_cb(int fd, short events,void *arg){
	evutil_socket_t sockfd;
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	sockfd = accept(fd,(struct sockaddr*)&client,&len);
	evutil_make_socket_nonblocking(sockfd);
	LOG(EVENT_LOG_MSG,"accept a client %d",sockfd);
	struct event_base* base = (struct event_base*)arg;
	struct event *ev = event_new(NULL,-1,0,NULL,NULL);
	event_assign(ev,base,sockfd,EV_READ|EV_PERSIST,socket_read_cb,(void*)ev);
	event_add(ev,NULL);
}
void socket_read_cb(int fd,short events,void *arg){
	char msg[4096];
	struct event *ev = (struct event*)arg;
	int len = read(fd,msg,sizeof(msg) - 1);
	if(len <= 0){
		LOG(EVENT_LOG_ERR,"some error happen when read");
		event_free(ev);
		close(fd);
		return;
	}
	msg[len] = '\0';
	printf("recv the client msg:%s",msg);
	LOG(EVENT_LOG_MSG,"recv the client msg:%s",msg);
	char reply_msg[4096] = "I have recvieced the msg:";
	strcat(reply_msg + strlen(reply_msg),msg);
	write(fd,reply_msg,strlen(reply_msg));
}
typedef struct sockaddr SA;
int tcp_server_init(int port, int listen_num)
{
    int errno_save;
    evutil_socket_t listener;
 
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if( listener == -1 )
        return -1;
 
    //允许多次绑定同一个地址。要用在socket和bind之间
    evutil_make_listen_socket_reuseable(listener);
 
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);
 
    if( bind(listener, (SA*)&sin, sizeof(sin)) < 0 )
        goto error;
 
    if( listen(listener, listen_num) < 0)
        goto error;
 
 
    //跨平台统一接口，将套接字设置为非阻塞状态
    evutil_make_socket_nonblocking(listener);
 
    return listener;
 
error:
    errno_save = errno;
    evutil_closesocket(listener);
    errno = errno_save;
    return -1;
}
/*--------------------- 
作者：luotuo44 
来源：CSDN 
原文：https://blog.csdn.net/luotuo44/article/details/39670221?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
 