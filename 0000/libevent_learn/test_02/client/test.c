#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <time.h>
#include <event2/event.h>
#include <string.h>//sleep
#include <unistd.h>
#include <stdlib.h>
#include <event.h>
#include <errno.h>
#include "../log/inc/log.h"

int tcp_connect_server(const char* server_ip, int port);
void cmd_msg_cb(int fd, short events, void* arg);
void socket_read_cb(int fd, short events, void *arg);

int main(int argc, char** argv)
{
    if( argc < 3 )
    {
        printf("please input 2 parameter\n");
        return -1;
    }
	init_log("/opt/client.log");
	LOG(EVENT_LOG_MSG,"begin");
    //两个参数依次是服务器端的IP地址、端口号
    int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));
    if( sockfd == -1)
    {
        LOG(EVENT_LOG_ERR,"tcp_connect error ");
        return -1;
    }
 
    LOG(EVENT_LOG_MSG,"connect to server successful\n");
 
    struct event_base* base = event_base_new();
 
    struct event *ev_sockfd = event_new(base, sockfd,
                                        EV_READ | EV_PERSIST,
                                        socket_read_cb, NULL);
    event_add(ev_sockfd, NULL);
 
    //监听终端输入事件
    struct event* ev_cmd = event_new(base, STDIN_FILENO,
                                      EV_READ | EV_PERSIST, cmd_msg_cb,
                                      (void*)&sockfd);
 
 
    event_add(ev_cmd, NULL);
 
    event_base_dispatch(base);
 
    LOG(EVENT_LOG_MSG,"finished \n");
	release_log();
    return 0;
}
void cmd_msg_cb(int fd, short events, void* arg)
{
    char msg[1024];
 
    int ret = read(fd, msg, sizeof(msg));
    if( ret <= 0 )
    {
        LOG(EVENT_LOG_ERR,"read fail ");
        exit(1);
    }
 
    int sockfd = *((int*)arg);
 
    //把终端的消息发送给服务器端
    //为了简单起见，不考虑写一半数据的情况
    write(sockfd, msg, ret);
}
 
 
void socket_read_cb(int fd, short events, void *arg)
{
    char msg[1024];
 
    //为了简单起见，不考虑读一半数据的情况
    int len = read(fd, msg, sizeof(msg)-1);
    if( len <= 0 )
    {
        LOG(EVENT_LOG_ERR,"read fail ");
        exit(1);
    }
 
    msg[len] = '\0';
	printf("recv %s from server\n", msg);
    LOG(EVENT_LOG_MSG,"recv %s from server\n", msg);
}
 
 
 
typedef struct sockaddr SA;
int tcp_connect_server(const char* server_ip, int port)
{
    int sockfd, status, save_errno;
    struct sockaddr_in server_addr;
 
    memset(&server_addr, 0, sizeof(server_addr) );
 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    status = inet_aton(server_ip, &server_addr.sin_addr);
 
    if( status == 0 ) //the server_ip is not valid value
    {
        errno = EINVAL;
        return -1;
    }
 
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if( sockfd == -1 )
        return sockfd;
 
 
    status = connect(sockfd, (SA*)&server_addr, sizeof(server_addr) );
 
    if( status == -1 )
    {
        save_errno = errno;
        close(sockfd);
        errno = save_errno; //the close may be error
        return -1;
    }
 
    evutil_make_socket_nonblocking(sockfd);
 
    return sockfd;
}
/*--------------------- 
作者：luotuo44 
来源：CSDN 
原文：https://blog.csdn.net/luotuo44/article/details/39670221?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
 