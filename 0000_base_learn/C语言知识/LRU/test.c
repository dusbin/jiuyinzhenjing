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
void _add(struct data *input,struct data *pre,struct data *next){
	pre->next = input;
	input->pre = pre;
	next->pre = input;
	input->next = next;
}
void add_tail(struct data *input,struct data *head){
	_add(input,head->pre,head);
}
void add_head(struct data *input,struct data *head){
	_add(input,head,head->next);
}
void _del(struct data *pre,struct data *next){
	pre->next = next;
	next->pre = pre;
}
void del(struct data *tmp){
	_del(tmp->pre,tmp->next);
}
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
			add_tail(add,g_list);
		}
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
		del(data);
		add_head(data,g_list);
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
	del(tmp);
	tmp->line = i;
	tmp->value = value;
	add_head(tmp,g_list);
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


 