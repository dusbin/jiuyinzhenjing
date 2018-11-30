#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void printWelcome(int len){
	printf("welcome--%d\n",len);
}
void printGoodbye(int len){
	printf("goodbye--%d\n",len);
}
void callback(int times,void(*print)(int)){
	int i;
	for(i = 0;i<times;++i){
		print(i);
	}
	printf("unknown welcome or goodbye\n");
}
typedef struct _OP{
	float (*p_add)(float,float);
	float (*p_sub)(float,float);
	float (*p_mul)(float,float);
	float (*p_div)(float,float);
}OP;
float ADD(float a,float b){
	return a+b;
}
float SUB(float a,float b){
	return a-b;
}
float MUL(float a,float b){
	return a*b;
}
float DIV(float a,float b){
	return a/b;
}
float operator(float a, float b, float (*op_func)(float,float)){//回调函数
	return (*op_func)(a,b);
}
void init_op(OP *op){
	op->p_add = ADD;
	op->p_sub = SUB;
	op->p_mul = MUL;
	op->p_div = DIV;
}
typedef struct list{
	void *value_address;
	struct list *next;
}NODE;
int int_compare(void const *a,void const *b){
	if(*(int*)a == *(int*)b){
		return 0;
	}else{
		return 1;
	}
}
int str_compare(void const *a,void const *b){
	if(0 == strcmp((char*)a,(char*)b)){
		return 0;
	}else{
		return 1;
	}
}
NODE *Search_List(NODE *node,void const *desired_value,int (*compare)(void const *,void const*)){
	while(node != NULL){
		if(compare((node->value_address),desired_value) == 0){//compare 回调函数
			break;
		}
		node = node->next;
	}
	return node;
}
typedef struct list_ex{
	int flag;
	void *value_address;
	struct list_ex *next;
}NODE_EX;
#define STRING 	0
#define INT		1
int Search(void const *node_value,void const *desired_value,int (*compare)(void const *,void const*)){
	if(compare(node_value,desired_value) == 0){//compare 回调函数
		return 0;
	}else{
		return 1;
	}
}
NODE_EX *Search_List_ex(NODE_EX *node,void const *desired_value,int flag){
	while(node != NULL){
		if(node->flag == STRING){
			if(flag != STRING){
				node = node->next;
				continue;
			}
			if(Search((node)->value_address,desired_value,str_compare) == 0){
				return node;
			}
		}else{
			if(flag != INT){
				node = node->next;
				continue;
			}
			if(Search((node)->value_address,desired_value,int_compare) == 0){
				return node;
			}
		}
		node = node->next;
	}
	return node;
}
//混合链，链表中含有int和string
void test_int_string(){
	NODE_EX node[10];
	int i = 0;
	int i_data[5] = {0,2,4,6,8};
	char str_data[5][20] = {"china","asia","shaanxi","xian","changan"};
	for(i = 0;i<10;i++){
		if(i<9){
			node[i].next = &node[i+1]; 
		}else{
			node[i].next = NULL;
		}
		if(i%2 == 0){
			node[i].value_address = &str_data[i/2];
			node[i].flag		  = STRING;
		}else{
			node[i].value_address = &i_data[i/2];
			node[i].flag		  = INT;
		}
	}
	for(i = 0;i<10;i++){
		if(node[i].flag == STRING){
			//printf("string %s\n",((char*)node[i].value_address));
		}else{
			//printf("int %d\n",*((int*)node[i].value_address));
		}
	}
	char str_search[20] = "shaanxi";
	NODE_EX *node_search = Search_List_ex(&node[0],&str_search,STRING);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %s\n",((char*)node_search->value_address));
	}else{
		printf("not find\n");
	}
	strcpy(str_search,"shanxi");
	node_search = Search_List_ex(&node[0],&str_search,STRING);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %s\n",((char*)node_search->value_address));
	}else{
		printf("not find\n");
	}
	int int_search = 2;
	node_search = Search_List_ex(&node[0],&int_search,INT);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %d\n",*((int*)node_search->value_address));
	}else{
		printf("not find\n");
	}
	int_search = 3;
	node_search = Search_List_ex(&node[0],&int_search,INT);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %d\n",*((int*)node_search->value_address));
	}else{
		printf("not find\n");
	}
}
void test_string(){
	NODE str_node[5];
	char str_data[5][20] = {"china","asia","shaanxi","xian","changan"};
	int i = 0;
	for(i = 0;i<5;i++){
		if(i<4){
			str_node[i].next = &str_node[i+1]; 
		}else{
			str_node[i].next = NULL;
		}
		str_node[i].value_address = &str_data[i];
	}
	char str_search[20] = "shaanxi";
	NODE *node_search = Search_List(&str_node[0],&str_search,str_compare);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %s\n",((char*)node_search->value_address));
	}else{
		printf("not find\n");
	}
	strcpy(str_search,"shanxi");
	node_search = Search_List(&str_node[0],&str_search,str_compare);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %s\n",((char*)node_search->value_address));
	}else{
		printf("not find\n");
	}
}
void test_int(){
	NODE i_node[5];
	int i_data[5] = {0,2,4,6,8};
	int i = 0;
	for(i = 0;i<5;i++){
		if(i<4){
			i_node[i].next = &i_node[i+1]; 
		}else{
			i_node[i].next = NULL;
		}
		i_node[i].value_address = &i_data[i];
	}
	int i_search = 2;
	NODE *node_search = Search_List(&i_node[0],&i_search,int_compare);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %d\n",*((int*)node_search->value_address));
	}else{
		printf("not find\n");
	}
	i_search = 3;
	node_search = Search_List(&i_node[0],&i_search,int_compare);
	if(node_search != NULL){
		printf("find\n");
		printf("node_search->value = %d\n",*((int*)node_search->value_address));
	}else{
		printf("not find\n");
	}
}
void test_callback(){
	callback(10,printWelcome);
	callback(10,printGoodbye);
	printWelcome(5);
}
void test_operator(){
	//使用回调函数
	printf("ADD = %f\n",operator(1.3,2.2,ADD));
	printf("SUB = %f\n",operator(1.3,2.2,SUB));
	printf("MUL = %f\n",operator(1.3,2.2,MUL));
	printf("DIV = %f\n",operator(1.3,2.2,DIV));
	//使用函数指针调用
	OP *op = (OP*)malloc(sizeof(OP));
	init_op(op);
	printf("ADD = %f\n",(op->p_add)(1.3,2.2));
	printf("SUB = %f\n",(op->p_sub)(1.3,2.2));
	printf("MUL = %f\n",(op->p_mul)(1.3,2.2));
	printf("DIV = %f\n",(op->p_div)(1.3,2.2));
	free(op);
	op = NULL;	
}
int main(){
	//test_callback();
	//test_operator();
	//test_int();
	//test_string();
	test_int_string();
	//回调3
	return 0;
}


 