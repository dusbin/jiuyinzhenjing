#include <stdio.h>
#include "jiafa.h"
#include "jianfa.h"
#include "chengfa.h"
#include "chufa.h"
#include "define.h"
int main(){
	int a,b;
	int ret=FUNC_INVALID;
	int soul=0;
	char c;
	int exit = 1;
	do{
		scanf("%d%c%d",&a,&c,&b);
		switch(c){
			case '+':
				ret = jiafa(a,b,&soul);
				break;
			case '-':
				ret = jianfa(a,b,&soul);
				break;
			case '*':
				ret = chengfa(a,b,&soul);
				break;
			case '/':
				ret = chufa(a,b,&soul);
				break;
			default:
				printf("exit\n");
				exit = 0;
			break;
		}
		if(FUNC_OK == ret){
			printf("%d %c %d = %d\n",a,c,b,soul);
		}else if(FUNC_INPUT_ERR == ret){
			printf("input error,[%d%c%d]\n",a,c,b);
		}else if(FUNC_INVALID == ret){
			printf("INVALID\n");
		}else if(FUNC_ERR == ret){
			printf("error\n");
		}
		
	}while(exit != 0);
	return 0;
}

