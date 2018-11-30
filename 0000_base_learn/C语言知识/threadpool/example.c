/*
 * example.c
 * Author : Herbert <wang_xiaoq@126.com>
 * LICENSE: GPL v2
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "thread-pool.h"

void test(void *arg)
{
    printf("I am a job\n");
    sleep(5);
}
void test1(void *arg)
{
    printf("I am a job2\n");
    sleep(5);
}
void test2(void *arg)
{
	char *a = (char *)arg;
    printf("I am a job3 %s\n",a);
    sleep(5);
}
int main(void)
{
    int ret = 0;
    int i = 0;
    pthread_t tid = 0;

    ret = thread_pool_init(3);
	char data[16] = "hello";
    for (i = 0; i < 100; i++) {
		switch(i%3){
			case 0: add_job_to_job_queue(test, NULL);break;
			case 1: add_job_to_job_queue(test1, NULL);break;
			case 2: add_job_to_job_queue(test2, data);break;
		}
    }

    sleep(10);

    /* destroy the thread pool immediately */
    thread_pool_destroy(0);
    /* wait all the job on the jon queue to complete */
    //thread_pool_destroy(1);

    return 0;
}
