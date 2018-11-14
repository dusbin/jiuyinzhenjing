#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
int main(){
	callback(10,printWelcome);
	callback(10,printGoodbye);
	printWelcome(5);
	return 0;
}


 