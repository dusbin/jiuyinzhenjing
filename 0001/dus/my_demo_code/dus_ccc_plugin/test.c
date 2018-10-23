#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
 
//动态链接库路径
#define LIB_MUL "./libmul.so" // '*'
#define LIB_SUB "./libsub.so" // '-'
#define LIB_ADD "./libadd.so" // '+'
#define LIB_DIV "./libdiv.so" // '/'
 //函数指针
typedef int (*CAC_FUNC)(int, int);

int main(){
    void *handle;
    char *error;
    CAC_FUNC cac_func = NULL;
    char lib_name[4][15] ={LIB_ADD,LIB_SUB,LIB_MUL,LIB_DIV};
    int a,b;
    char c;
	int opt_flag;
	int exit_flag = 1;
	do{
		scanf("%d%c%d",&a,&c,&b);
		switch(c){
			case '+':
				opt_flag = 0;
				handle = dlopen(lib_name[opt_flag], RTLD_LAZY);
				break;
			case '-':
				opt_flag = 1;
				handle = dlopen(lib_name[opt_flag], RTLD_LAZY);
				break;
			case '*':
				opt_flag = 2;
				handle = dlopen(lib_name[opt_flag], RTLD_LAZY);
				break;
			case '/':
				opt_flag = 3;
				handle = dlopen(lib_name[opt_flag], RTLD_LAZY);
				break;
			default:
				printf("exit\n");
				exit_flag = 0;
			break;
		}
		if(exit_flag != 0){
			if (!handle) {
				printf("load %s error!\n",lib_name[opt_flag]);
				goto WARNING;
			}
			//清除之前存在的错误
		
			dlerror(); 
			//获取函数
			*(void **) (&cac_func) = dlsym(handle, "func");
			printf("%d %c %d = %d\n",a,c,b,(*cac_func)(a,b));
			//关闭动态链接库
		WARNING:
			if (handle)
				dlclose(handle);
		}
	}while(exit_flag != 0);
	return 0;
}

