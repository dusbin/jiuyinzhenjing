#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"
typedef struct unit{
	struct list_head element; //链表节点
	char name[24]; //数据元
	int id;//数据元
}unit;
int main(){
	struct list_head list;
	INIT_LIST_HEAD(&list);
	if(list_empty(&list)){
		printf("list is empty!\n");
	}
	unit* unit_add = NULL;
	unit_add = (unit*)malloc(sizeof(unit));
	if(unit_add == NULL){
		return -1;
	}
	unit_add->id = 1;
	strcpy(unit_add->name,"zhangsan");
	list_add_tail(&unit_add->element,&(list));//添加到尾
	unit_add = NULL;
	if(list_empty(&list)){
		printf("list is empty!\n");
	}
	unit_add = (unit*)malloc(sizeof(unit));
	if(unit_add == NULL){
		return -1;
	}
	unit_add->id = 2;
	strcpy(unit_add->name,"lisi");
	list_add_tail(&unit_add->element,&(list));//添加到尾
	if(list_empty(&list)){
		printf("list is empty!\n");
	}
	unit_add = NULL;
	unit_add = (unit*)malloc(sizeof(unit));
	if(unit_add == NULL){
		return -1;
	}
	unit_add->id = 3;
	strcpy(unit_add->name,"wangwu");
	list_add(&unit_add->element,&(list));//添加到尾
	unit_add = NULL;
	struct list_head* pos = NULL,*n = NULL;
	unit* unit_tmp = NULL;
	list_for_each_safe(pos,n,&list)
    {
        unit_tmp = container_of(pos, unit, element);
		printf("name :[%s],id: [%d]\n",unit_tmp->name,unit_tmp->id);
    }
	printf("del one node!\n");
	int i = 1;
	list_for_each_safe(pos,n,&list)
    {
		if(i == 2){
			unit_tmp = container_of(pos, unit, element);
			list_del(pos);
			free(unit_tmp);
		}
		i++;
    }
	list_for_each_safe(pos,n,&list)
    {
        unit_tmp = container_of(pos, unit, element);
		printf("name :[%s],id: [%d]\n",unit_tmp->name,unit_tmp->id);
    }
	return 0;
}