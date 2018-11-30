#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; //初始化条件变量
void *thread1(void *);
void *thread2(void *);
int i=1;
int main(void){
	pthread_t t_a;
	pthread_t t_b;
	pthread_create(&t_a,NULL,thread1,(void *)NULL);/*创建进程t_a*/
	pthread_create(&t_b,NULL,thread2,(void *)NULL); /*创建进程t_b*/
	pthread_join(t_b, NULL);/*等待进程t_b结束*/
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	exit(0);
}
void *thread1(void *junk){
	for(i=1;i<=9;i++){
		printf("IN one\n");
		pthread_mutex_lock(&mutex);//
		if(i%3==0)
			pthread_cond_signal(&cond);/*条件改变，发送信号，通知t_b进程*/
		else
			printf("thead1:%d\n",i);
		pthread_mutex_unlock(&mutex);//*解锁互斥量*/
		printf("Up Mutex\n");
		sleep(10);
	}
}
void *thread2(void *junk){
	while(i<9){
		printf("IN two \n");
		pthread_mutex_lock(&mutex);
		if(i%3!=0)
			pthread_cond_wait(&cond,&mutex);/*等待*/
		printf("thread2:%d\n",i);
		pthread_mutex_unlock(&mutex);
		printf("Down Mutex\n");
		sleep(10);
	}
}

