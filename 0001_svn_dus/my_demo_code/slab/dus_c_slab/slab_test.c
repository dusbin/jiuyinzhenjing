#include<stdio.h>
#include<comm_slab.h>


int main(){
	printf("test begin...\n");
	int max = 10,size = 1024;
	struct uxsino_slab g_slab;
	INIT_SLAB(g_slab,max,size);
	printf("test end...\n");
	return 0;
}

/*
int a=1, b=2;
int fun(int c,int d){
	int e=0;
	e=c+d;
	return e;
}
int main(){
	printf("a+b=%d\n",fun(a,b));
	a++;b=4;
	printf("a+b=%d\n",fun(a,b));
}
*/
