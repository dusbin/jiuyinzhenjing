#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
struct data{
	struct data *right,*left,*parent;
	int value;
};
int max = 0;
void insert(const int value,struct data *stack){
	if(stack == NULL){
		return;
	}
	if(stack->value == 0){//根节点没有数据
		stack->value = value;
		return;
	}
	if(stack->right == NULL){
		struct data *add = (struct data*)malloc(sizeof(struct data));
		if(add == NULL){
			return;
		}
		add->value = value;
		add->parent = stack;
		stack->right = add;
		add->right = NULL;
		add->left = NULL;
		return;
	}
	if(stack->left == NULL){
		struct data *add = (struct data*)malloc(sizeof(struct data));
		if(add == NULL){
			return;
		}
		add->value = value;
		add->parent = stack;
		stack->left = add;
		add->right = NULL;
		add->left = NULL;
		return;
	}
	struct data *add = (struct data*)malloc(sizeof(struct data));
	if(add == NULL){
		return;
	}
	add->value = value;
	
	
}
void TopN(int a[],struct data *stack){
	printf("a %p\n",a);
	//int max = sizeof(a)/sizeof(int);
	//for(int i = 0;i < max;i++){
		//insert(a[i],&stack);
	///}
	printf("max = %d\n",max);
	return;
}
void init_stack(struct data *stack){
	stack->right = NULL;
	stack->left = NULL;
	stack->parent = NULL;
	stack->value = 0;
}
//大顶堆
void TopN1(int a[]){
	printf("a %p\n",a);
	int tmp = 0;
	int left,right,flag;
	do{//不停的对堆进行调整，如果某一轮没有调整，即堆已经成为大顶堆
		flag = 0;
		for(int i = 0;i < max;i++){
			left = 2*i;
			right = 2*i +1;
			if(left < max){//左子存在
				if(right < max){//右子存在
					if(a[i] < a[left]||a[i]<a[right]){
						if(a[left]<a[right]){//与右支交换
							if(a[i]<a[right]){
								tmp = a[right];
								a[right] = a[i];
								a[i] = tmp;
								flag = 1;
							}
						}else{//与左支交换
							if(a[i]<a[left]){
								tmp = a[left];
								a[left] = a[i];
								a[i] = tmp;
								flag = 1;
							}
						}
					}
				}else{//右支不存在
					if(a[i] < a[left]){
						tmp = a[left];
						a[left] = a[i];
						a[i] = tmp;
						flag = 1;
					}
				}
			}
		}
	}while(flag != 0);
	return;
}
/*小顶堆*/
void TopN2(int a[]){
	printf("a %p\n",a);
	int tmp = 0;
	int left,right,flag;
	do{//不停的对堆进行调整，如果某一轮没有调整，即堆已经成为大顶堆
		flag = 0;
		for(int i = 0;i < max;i++){
			left = 2*i;
			right = 2*i +1;
			if(left < max){//左子存在
				if(right < max){//右子存在
					if(a[i] > a[left]||a[i]>a[right]){
						if(a[left]>a[right]){//与右支交换
							if(a[i]>a[right]){
								tmp = a[right];
								a[right] = a[i];
								a[i] = tmp;
								flag = 1;
							}
						}else{//与左支交换
							if(a[i]>a[left]){
								tmp = a[left];
								a[left] = a[i];
								a[i] = tmp;
								flag = 1;
							}
						}
					}
				}else{//右支不存在
					if(a[i] > a[left]){
						tmp = a[left];
						a[left] = a[i];
						a[i] = tmp;
						flag = 1;
					}
					
				}
			}
		}
	}while(flag != 0);
	return;
}
void print_data(int a[]){
	for(int i = 0;i< max;i++){
		printf("%d ",a[i]);
	}
	printf("\n");
}
int main(){
	int a[] = {1,2,3,4,5,6,7,8,9};//14
	max = sizeof(a)/sizeof(int);
	struct data stack;
	init_stack(&stack);
	TopN(&a,&stack);
	//print_data(a);
	TopN1(a);
	print_data(a);
	TopN2(a);
	print_data(a);
	return 0;
}


 