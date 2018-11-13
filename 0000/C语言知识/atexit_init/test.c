#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
__attribute((constructor)) void before_main(){
	printf("before main\n");
}
__attribute((destructor)) void after_main(){
	printf("after main\n");
}
void test1(){
	sleep(1);
	printf("test1\n");
}
void test2(){
	printf("test2\n");
}
int main()
{
	printf("main\n");
	atexit(test1);
	atexit(test2);
	return 0;
}

