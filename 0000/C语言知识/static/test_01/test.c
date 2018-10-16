#include <stdio.h>
void fn(void){
	int n = 10; 
	printf("n=%d\n", n); 
	n++; 
	printf("n++=%d\n", n); 
} 
void fn_static(void){ 
	static int n = 10; //退出后不释放，栈空间分配，在递归中使用有特殊效果
	printf("static n=%d\n", n); 
	n++; 
	printf("n++=%d\n", n); 
}
int main(){
	fn(); 
	printf("--------------------\n"); 
	fn_static(); 
	printf("--------------------\n"); 
	fn(); 
	printf("--------------------\n"); 
	fn_static();
	return 0;
}


 