#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uxmd5.h"
int enc_data(const char* number,char* enc,int len_enc){
	char md5[33] = {0};
	char double_md5[65] = {0};
	stringmd5(number,md5);
	printf("md5 = %s\n",md5);
	unsigned long num = atol(number);
	unsigned long offset = 0;
	offset = num%len_enc;
	strcpy(double_md5,md5);
	strcat(double_md5,md5);
	strncpy(enc,double_md5+offset,len_enc);
	return 0;
}
int enc_phone_number(char* number,int len_enc){
	char enc[32] = {0};
	int len = strlen(number);
	if(len < len_enc ||len_enc > 32){
		return -1;
	}
	memset(enc,0x00,32);
	enc_data(number,enc,len_enc);
	strcpy(number+(len-len_enc),enc);
	return 0;
}
int main(){
	char number[12]={0};
	strcpy(number,"18792741184");
	printf("beore enc phone:[%s]\n",number);
	enc_phone_number(number,3);
	printf("after enc phone:[%s]\n",number);
	printf("--------------------------------\n");
	strcpy(number,"18792741184");
	printf("beore enc phone:[%s]\n",number);
	enc_phone_number(number,4);
	printf("after enc phone:[%s]\n",number);
	printf("--------------------------------\n");
	strcpy(number,"18792741184");
	printf("beore enc phone:[%s]\n",number);
	enc_phone_number(number,5);
	printf("after enc phone:[%s]\n",number);
	return 0;
}