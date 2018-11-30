#include<stdio.h>
int main(){
	printf("test\n");
	#ifdef DUS
	printf("DUS\n");
	#endif
	#ifdef UXSINO
	printf("UXSINO\n");
	#endif
	return 0;
}