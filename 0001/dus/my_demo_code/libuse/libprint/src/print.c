#include <add.h>
#include <sub.h>
#include <stdio.h>
int print(int a,int b,char i){
	switch(i){
		case '+':
			printf("%d + %d = %d\n",a,b,add(a,b));
		break;
		case '-':
			printf("%d - %d = %d\n",a,b,sub(a,b));
			break;
		default:
			printf("opt err\n");
			break;
	}
	return 0;
}

