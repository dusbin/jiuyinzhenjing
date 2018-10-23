#include<stdio.h>
#include<stdlib.h>
#include "sms4.h"
int main(){
	char str[4096] = {0};
	char stren[4096] = {0};
	char strde[4096] = {0};
	int len = 0;
	strcpy(str,"duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\n\
duzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid\nduzhengbin xianyang weicheng hid");
	printf("before encrypt\n");
	printf("str = %s\n",str);
	sms4_data_encrypt(str,strlen(str),stren,strlen(str));
	printf("------------------------------------------\nstren = %s\n",stren);
	sms4_data_decrypt(stren,strlen(str),strde,strlen(str));
	printf("------------------------------------------\nstrde = %s\n",strde);
	return 0;
}

