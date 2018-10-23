#include <stdio.h>
#include "thread_pool.h"
#include <unistd.h>
#include <pthread.h>
struct thread_pool pool={0};
#if 1
void* fun(void* p)
{
	printf("thread_id = 0x%lx is running!\n",pthread_self());
	sleep(10);
	return 0;
}
int main()
{
	int i= 20;
	int ret = -1;
	printf("start!!!!\n");
	struct thread_pool * threadpool= NULL;
	threadpool = thread_pool_create(10);
	printf("@@@thread_num = %d\n",threadpool->thread_num);
	printf("@@@task_num = %d\n",threadpool->task_num);
	printf("@@@curr_running_task_num = %d\n",threadpool->curr_running_task_num);

	sleep(5);
	if(threadpool == NULL)
	{
		printf("threadpool = NULL error!\n");
	}
	for (i = 0; i < 10; i++)
	{
		ret = thread_pool_add_task(threadpool,fun,NULL);
		if(ret == -1)
		{
			printf("thread_pool_add_task error!   i=%d\n",i);
		}
		printf("i========%d!!!!\n",i);
	}
	while(threadpool->task_num) {
		sleep(1);
	}
	while (threadpool->curr_running_task_num) {
		printf("----------------curr_running_task_num = %d\n",threadpool->curr_running_task_num);
		sleep(1);
	}

	thread_pool_release(threadpool);

	return 0;
}

#endif

