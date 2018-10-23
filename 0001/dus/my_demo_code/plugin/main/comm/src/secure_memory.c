#include <stdio.h>
#include <pthread.h>
#include "secure_memory.h"

void* malloc_s(unsigned size){
	return malloc(size);
}
long getdate();
void free_s(void *p){
	memory_unit* memory_add = (memory_unit*)malloc(sizeof(memory_unit));
	memory_add->p = p;
	memory_add->time = getdate();
	printf("memory_add->time  = %ld\n",memory_add->time);
	list_add_tail(&memory_add->element,&(g_memory_list));
	return;
}

void free_memory(){
	long nowtime = getdate();
	printf("now_time = %ld\n",nowtime);
	printf("--------begin printf--------\n");
	if(list_empty(&g_memory_list)){
		printf("list_empty\n");
		printf("---------end printf---------\n");
		return;
	}
	struct list_head* pos = NULL,*n = NULL;
	memory_unit* memory_unit_tmp = NULL;
	list_for_each_safe(pos,n, &g_memory_list)
    {
        memory_unit_tmp = container_of(pos, memory_unit, element);
		if((nowtime - memory_unit_tmp->time) > MAX_TIME){
			list_del(pos);
			free(memory_unit_tmp->p);
			memory_unit_tmp->p = NULL;
			free(memory_unit_tmp);
			memory_unit_tmp = NULL;
		}
    }
	printf("---------end printf---------\n");
	return;
}
static void *thrd_monitor_memory(){
	INIT_LIST_HEAD(&g_memory_list);
	for(;;){
		free_memory();
		sleep(100);//每100秒释放一次
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return NULL;
}
int monitor_memory(){
	pthread_t thrd_id;
	pthread_create(&thrd_id,NULL,thrd_monitor_memory,NULL);
	return FUNC_OK;
}
int memory_test(){
	int a = 2000;
	char *p;
	do{
		//scanf("%d",&a);
		if(p != NULL){
			p = NULL;
		}
		p = malloc_s(1024*32);
		free_s(p);
		printf("before strcpy\n");
		strcpy(p,"name:duzhengbin tel:18792741184");
		printf("p->%s\n",p);
		sleep(90);
		printf("90s...");
		printf("p->%s\n",p);
		sleep(20);
		printf("110s...");
		printf("p->%s\n",p);
		sleep(90);
		printf("200s...");
		printf("p->%s\n",p);
		p = NULL;
		sleep(1);
	}while(a-- > 0);
}

long getdate(){
	return (long)time(0);
}

