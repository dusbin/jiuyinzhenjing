#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
   //char a[]="Password";        //要加密的密码
   char b[]="encryption";     //密钥
   //int i;
   //加密代码
   char *a;
   get_file_data("cacert.pam",&a);
   printf("You Password: %s\n",a);
   encrypt(a,b);
   //set_file_data("cacert.pam.mi",&a);
   //free(a);
   //a=NULL;
   //get_file_data("cacert.pam.mi",&a);
   encrypt(a,b);
   printf("Your Password is encrypted: %s\n",a);
   //free(a);
   //a=NULL;
	return 0;
}
int encrypt(char *data,char *key){
	int data_len = strlen(data);
	int key_len = strlen(key);
	int i = 0;
	for(i=0;data[i];i++)
     data[i]=data[i]^key[i%key_len];
}
int decrypt(){
	
}
int set_file_data(const char *path,char *data){
	FILE *fp = NULL;
	int len = 0;
	fp = fopen(path,"w+");
	len = strlen(data);
	fwrite(data,len,1,fp);
	fclose(fp);
	fp = NULL;
	return 0;
}
int get_file_data(const char *path,char **data){
	FILE *fp = NULL;
	int len = 0;
	fp=fopen(path,"rb");
	if(NULL == fp){
		goto ERR_END;
	}
	fseek(fp,0,SEEK_END);	
	len=ftell(fp);	
	fseek(fp,0,SEEK_SET);	
	*data=(char*)malloc(len+1);
	if(NULL == *data){
		fclose(fp);
		return -1;
	}
	memset(*data,0,len+1);	
	fread(*data,1,len,fp);	
	fclose(fp);
	fp = NULL;	
ERR_END:
	return 0;
}



