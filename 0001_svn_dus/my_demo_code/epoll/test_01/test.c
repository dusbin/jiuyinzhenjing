#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
int main(){
	int epfd, nfds;
	struct epoll_event ev,events[5];
	epfd = epoll_create(1);
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN|EPOLLET;
	//ev.events = EPOLLIN;//默认使用LT模式
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);
	for(;;){
		nfds = epoll_wait(epfd,events,5,-1);
		for(int i = 0; i < nfds; i++){
			//char buff[1024] = {0};
			//read(STDIN_FILENO,buff,sizeof(buff));
			if(events[i].data.fd == STDIN_FILENO){
				printf("welcome to epoll's world!\n");
				ev.data.fd = STDIN_FILENO;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_MOD,STDIN_FILENO,&ev);
			}
		}
	}
	return 0;
}