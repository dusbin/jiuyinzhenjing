#ifndef _COMM_SLAB_H_
#define _COMM_SLAB_H_
//#include "simple_list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SLAB_MAX_NUM 32
#define SLAB_SIZE 32
typedef struct _tag_mem_unit{
	/*const*/ char * p;
	struct _tag_mem_unit* next;
}mem_unit;
struct uxsino_slab_ops;
//struct uxsino_slab;
typedef struct _tag_uxsino_slab{
	//int m_unit_max_num;/*缓冲区块数,创建时由调用者输入*/
	//int m_unit_size;/*缓冲区块数*/
	mem_unit m_mem_uns[SLAB_MAX_NUM];
	//struct list_head m_free_mem_objs;/*free*/
    //struct list_head m_used_mem_objs;/*used*/
	struct _tag_mem_unit* m_free_mem_objs;
	struct _tag_mem_unit* m_used_mem_objs;
	struct uxsino_slab_ops* ops;/*缓冲区块操作*/
	char m_mem_unit[SLAB_MAX_NUM*SLAB_SIZE];
}uxsino_slab;
/**函数*/
int create_slab(/* struct */ uxsino_slab* slab /*,int slab_max_num,int slab_size */);
int release_slab(/*struct */uxsino_slab* slab);
char* alloc_slab(uxsino_slab* slab);
void free_slab(uxsino_slab* slab,char* ptr);
/**缓冲区操作(函数)结构*/
struct uxsino_slab_ops{
	int (*create_slab)(uxsino_slab* slab/*,int slab_max_num,int slab_size*/);
	int (*release_slab)(uxsino_slab* slab);
	char* (*alloc_slab)(uxsino_slab* slab);
	void (*free_slab)(uxsino_slab* slab,char* ptr);
};
/**全局变量g_uxsino_slab_ops*/
/*
struct uxsino_slab_ops g_uxsino_slab_ops = {
	.create_slab = create_slab,
	.release_slab = release_slab,
	.alloc_slab = alloc_slab,
	.free_slab = free_slab,
};
*/
/*
 *初始化slab
 *参数slab [ops] max size 自身，块数，块大小
 */

//int INIT_SLAB(struct uxsino_slab* slab /*,struct uxsino_slab_ops* ops,int max,int size */){
//	//assert(slab /*&& g_uxsino_slab_ops*/);
//	slab.ops = g_uxsino_slab_ops;
//	slab->ops.create(slab/*,max,size*/);//创建，参数为自身，块数，块大小
//	return 0;
//}

int create_slab(/* struct */ uxsino_slab* slab /*,int slab_max_num,int slab_size */){
	memset(slab->m_mem_unit,0x00,SLAB_MAX_NUM*SLAB_SIZE);
	//INIT_LIST_HEAD(&slab->m_free_mem_objs);
	//INIT_LIST_HEAD(&slab->m_used_mem_objs);
	for(int i = 0;i < SLAB_MAX_NUM;i++){
		slab->m_mem_uns[i].p = (char*)(slab->m_mem_unit+i*SLAB_SIZE);
		slab->m_mem_uns[i].next = &slab->m_mem_uns[i+1];
	}
	slab->m_mem_uns[SLAB_MAX_NUM-1].next = NULL;
	slab->m_free_mem_objs = &slab->m_mem_uns[0];
	slab->m_used_mem_objs = NULL;
	return 0;
}
int release_slab(/*struct */ uxsino_slab* slab){
	//free(slab);
	return 0;
}
char* alloc_slab(uxsino_slab* slab){
	if(NULL == slab->m_free_mem_objs){
		return NULL;
	}
	struct _tag_mem_unit* free = slab->m_free_mem_objs;
	slab->m_free_mem_objs = slab->m_free_mem_objs->next;
	free->next = slab->m_used_mem_objs;
	slab->m_used_mem_objs = free;
	return free->p;
}
void free_slab(uxsino_slab* slab,char* ptr){
	if(ptr == NULL){
		return;
	}
	if(NULL == slab->m_used_mem_objs){
		if(ptr != NULL){
			free(ptr);
			ptr = NULL;
		}
		return;
	}
	struct _tag_mem_unit* temp = slab->m_used_mem_objs;
	if(temp->p == ptr){
		memset(ptr,0x00,SLAB_SIZE);
		strcpy(ptr,"free267890abcdefghijklmnopqr");
		slab->m_used_mem_objs = slab->m_used_mem_objs->next;
		temp->next = slab->m_free_mem_objs;
		slab->m_free_mem_objs = temp;
		ptr = NULL;
		return;
	}
	struct _tag_mem_unit* temp1 = temp->next;
	do{
		if(temp1->p == ptr){
			memset(ptr,0x00,SLAB_SIZE);
			strcpy(ptr,"free167890abcdefghijklmnopqr");
			temp->next = temp->next->next;
			temp1->next = slab->m_free_mem_objs;
			slab->m_free_mem_objs = temp1;
			ptr = NULL;
			return;
		}
		temp = temp->next;
		temp1 = temp->next;
	}while(temp1 == NULL);
	if(ptr != NULL){
		free(ptr);
		ptr = NULL;
	}
	return;
}
#endif