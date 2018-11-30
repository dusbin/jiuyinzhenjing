#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "EncodeBase64.h"
int main(){
	char buf[64] = {0};
	char base64[64] = {0};
	char debase64[64] = {0};
	int len = 0;
	strcpy(buf,"duzhengbin");
	printf("buf %s\n",buf);
	len = strlen(buf);
	base64_encode(buf,base64,len);
	printf("base64 %s\n",base64);
	base64_decode(base64, debase64);
	printf("debase64 %s\n",debase64);
	return 0;
}

