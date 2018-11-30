#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
//pthread_t ntid;

void *thr_fn1(void *arg){
	printf("thread 1 returning\n");
	return (void *)1;
}
void *thr_fn2(void *arg){
	while(1){
		printf("thread 2 returning\n");
		sleep(1);
	}
	//pthread_exit ((void *)2);
}
void *thr_fn3(void *arg){
	printf("thread 3 returning\n");
	pthread_exit ((void *)2);
}
int main(){
	printf("begin\n");
	pthread_t tid;
	void *tret;
	int err;
	pthread_create(&tid,NULL,thr_fn1,NULL);
	pthread_join(tid,&tret);
	printf("thread 1 exit code %d\n",(int)tret);
	
	pthread_create(&tid,NULL,thr_fn2,NULL);
	pthread_join(tid,&tret);//挂起该线程
	printf("thread 2 exit code %d\n",(int)tret);

	pthread_create(&tid,NULL,thr_fn3,NULL);
	sleep(3);
	pthread_cancel(tid);//取消tid线程
	pthread_join(tid,&tret);
	printf("thread 3 exit code %d\n",(int)tret);
	printf("end\n");
	return 0;
}

