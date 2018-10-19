#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 5
int get_from_file(int idx){
	char cmd[1024] = {0};
	sprintf(cmd,"sed -n '%d,1p' data.txt",idx);
	FILE *fp = popen(cmd,"r");
	if(fp == NULL)
		return -1;
	fgets(cmd,sizeof(cmd),fp);
	pclose(fp);
	fp = NULL;
	return atoi(cmd);
}
struct data{
	struct data* next;
	struct data* pre;
	int line;
	int value;
};
struct data* g_list = NULL;
void init_list(){
	int i = N;
	struct data* tmp;
	do{
		struct data* add = (struct data*)malloc(sizeof(struct data));
		add->next = add;
		add->pre = add;
		add->line = 0;
		add->value = -1;
		if(g_list == NULL){
			tmp = add;
			g_list = tmp;
			tmp->line = -1;
		}else{
			tmp->next = add;
			add->next = g_list;
			g_list->pre = add;
			add->pre = tmp;
			tmp = tmp->next;
		}
		printf("111\n");
		i--;
	}while(i >= 0);
}
void print_list(){
	struct data *tmp;
	tmp = g_list->next;
	if(tmp == NULL){
		return;
	}
	while(tmp!=g_list){
		//printf("tmp = %p,tmp->pre = %p, tmp->next = %p, line = %d, value = %d\n",tmp,tmp->pre,tmp->next,tmp->line,tmp->value);
		printf("line = %d, value = %d\n",tmp->line,tmp->value);
		tmp = tmp->next;
	}
	//printf("g_list = %p\n",g_list);
	return;
}
void update_to_head(struct data *data){
		struct data* tmp;
		tmp = data;
		tmp->pre->next = tmp->next;
		tmp->next->pre = tmp->pre;		
		tmp->next = g_list->next;
		tmp->pre = g_list;
		g_list->next->pre = tmp;
		g_list->next = tmp;
}
int get_data_from_cache(int idx){
	struct data * tmp;
	tmp = g_list->next;
	if(tmp == NULL){
		return -1;
	}
	int value = -1;
	while(tmp!=g_list){
		if(tmp->line == idx){
			value = tmp->value;
			update_to_head(tmp);
			return value;
		}
		tmp = tmp->next;
	}
	return -1;
}
void update_from_file_to_cache(int i,int value){
	struct data * tmp = g_list->pre;
	g_list->pre = tmp->pre;
	g_list->pre->next = g_list;
	tmp->line = i;
	tmp->value = value;
	g_list->next->pre = tmp;
	tmp->next = g_list->next;
	tmp->pre = g_list;
	g_list->next = tmp;
}
int main(){
	int i = -1;
	init_list();
	int value;
	print_list();
	while(1){
		//scanf("%d",&i);
		i = rand()%19+1;
		if(i > 0){
			value = get_data_from_cache(i);
			printf("value = %d\n",value);
			if(value < 0 ){
				value = get_from_file(i);
				update_from_file_to_cache(i,value);
			}
		}else if(i == 0){
			return 0;
		}else{
			printf("input error");
		}
		print_list();
		sleep(1);
	}
	return 0;
}


 