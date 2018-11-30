// slab.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
//#include "simple_list.h"
#include "comm_slab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
uxsino_slab g_slab;
void print(mem_unit mem[]){
	int i = 0;
	for(i = 0; i<32;i++){
		printf("%0.32s\n",mem[i].p);
	}
	printf("--------------------------------\n");
}
void print_point(mem_unit mem[]){
	printf("print by point\n");
	mem_unit* temp;
	temp = &mem[0];
	if(NULL == temp)
		return;
	int i = 0;
	do{
		i++;
		printf("%d %0.32s\n",i,(*temp).p);
		temp = temp->next;
	}while(NULL != temp);
	printf("--------------------------------\n");
}
void print_list(struct _tag_mem_unit* list){
	printf("--------print list--------------\n");
	if(NULL == list){
		printf("list is empty\n");
		return;
	}
	struct _tag_mem_unit* temp = list;
	do{
		printf("%0.32s\n",(*temp).p);
		temp = temp->next;
	}while(NULL != temp);
	printf("--------------------------------\n");
};
int main(){
	printf("test begin...\n");
	printf("--------------------------------\n");
	create_slab(&g_slab);
	//g_slab.ops = &g_uxsino_slab_ops;
	//g_slab.ops.create_slab(g_slab);
	printf("create_slab_end\n");
	strcpy(g_slab.m_mem_unit,"\
1234567890abcdefghijklmnopqrs00:\
1234567890abcdefghijklmnopqrs01:\
1234567890abcdefghijklmnopqrs02:\
1234567890abcdefghijklmnopqrs03:\
1234567890abcdefghijklmnopqrs04:\
1234567890abcdefghijklmnopqrs05:\
1234567890abcdefghijklmnopqrs06:\
1234567890abcdefghijklmnopqrs07:\
1234567890abcdefghijklmnopqrs08:\
1234567890abcdefghijklmnopqrs09:\
1234567890abcdefghijklmnopqrs10:\
1234567890abcdefghijklmnopqrs11:\
1234567890abcdefghijklmnopqrs12:\
1234567890abcdefghijklmnopqrs13:\
1234567890abcdefghijklmnopqrs14:\
1234567890abcdefghijklmnopqrs15:\
1234567890abcdefghijklmnopqrs16:\
1234567890abcdefghijklmnopqrs17:\
1234567890abcdefghijklmnopqrs18:\
1234567890abcdefghijklmnopqrs19:\
1234567890abcdefghijklmnopqrs20:\
1234567890abcdefghijklmnopqrs21:\
1234567890abcdefghijklmnopqrs22:\
1234567890abcdefghijklmnopqrs23:\
1234567890abcdefghijklmnopqrs24:\
1234567890abcdefghijklmnopqrs25:\
1234567890abcdefghijklmnopqrs26:\
1234567890abcdefghijklmnopqrs27:\
1234567890abcdefghijklmnopqrs28:\
1234567890abcdefghijklmnopqrs29:\
1234567890abcdefghijklmnopqrs30:\
1234567890abcdefghijklmnopqrs31:");	
	printf("g_slab.m_mem_uns\n");
	print(g_slab.m_mem_uns);
	print_point(g_slab.m_mem_uns);
	release_slab(&g_slab);
	print_list(g_slab.m_free_mem_objs);
	print_list(g_slab.m_used_mem_objs);
	char* mem_slab = alloc_slab(&g_slab);
	strcpy(mem_slab,"2017_10_17_10_20");
	print_point(g_slab.m_mem_uns);
	printf("g_slab.m_free_mem_objs\n");
	print_list(g_slab.m_free_mem_objs);
	printf("g_slab.m_used_mem_objs\n");
	print_list(g_slab.m_used_mem_objs);
	printf("%0.32s\n",mem_slab);
	char* mem_slab1 = alloc_slab(&g_slab);
	strcpy(mem_slab1,"2017_10_17_10_29");
	print_point(g_slab.m_mem_uns);
	printf("g_slab.m_free_mem_objs\n");
	print_list(g_slab.m_free_mem_objs);
	printf("g_slab.m_used_mem_objs\n");
	print_list(g_slab.m_used_mem_objs);
	print(g_slab.m_mem_uns);
	printf("free_slab\n");
	free_slab(&g_slab,mem_slab);
	mem_slab=NULL;
	free_slab(&g_slab,mem_slab1);
	free_slab(&g_slab,mem_slab);
	strcpy(g_slab.m_mem_uns[2].p,"freed67890abcdefghijopqrs01:");
	print(g_slab.m_mem_uns);
	printf("test end...\n");
	system("pause");
 	return 0;
}