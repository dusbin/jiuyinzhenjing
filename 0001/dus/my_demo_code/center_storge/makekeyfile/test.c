#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdlib.h>
#include "License.h"
#define DATA_PATH "user.data"
#define PRIVATE_PATH "privkey.pem"
#define CACERT_PATH "cacert.pem"
#define PRIVATE_BAK_PATH "privkey.pem.bak"
#define CACERT_BAK_PATH "cacert.pem.bak"
int get_config();
int set_config();
int get_file_data(const char * path,char ** data);
int main(){
	set_config();
	get_config();
}
int set_config(){
	int state = 0;
	char *data = NULL;
	cJSON *root = NULL,*obj=NULL,*result=NULL;
	char cmd[1024] = {0};
	int len = 0;
	if(0==access(DATA_PATH)){
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd,"cat /dev/null > \"%s\"; touch \"%s\"",DATA_PATH,DATA_PATH);
		system(cmd);
	}
	memset(cmd,0x00,sizeof(cmd));
	sprintf(cmd,"touch \"%s\"",DATA_PATH);
	system(cmd);
	root = cJSON_CreateObject();
    if (NULL == root) goto ERR_END;
	cJSON_AddStringToObject(root, "version", "ver:0.0.0.1");
	// get private.pem data
	if(-1 == get_file_data(PRIVATE_PATH,&data)){
		goto ERR_END;
	}
	cJSON_AddStringToObject(root, "private.pem", data);
	if(NULL != data){
		free(data);
		data = NULL;
	}
	// get cacert.pem data
	if(-1 == get_file_data(CACERT_PATH,&data)){
		goto ERR_END;
	}
	cJSON_AddStringToObject(root, "cacert.pem", data);
	if(NULL != data){
		free(data);
		data = NULL;
	}
	//end
	data = cJSON_PrintUnformatted(root);
	if(NULL!=root){
		cJSON_Delete(root);
	}
	//encrypt
	PAIRKEY g_pair_key;
	GeneratePublicKey(&g_pair_key);
	encrypt_license_file(&g_pair_key,data);
	//write to file
	//set_file_data(DATA_PATH,data);
	if(NULL != data){
		free(data);
		data = NULL;
	}
	return 0;
ERR_END:
	if(NULL!=root){
		cJSON_Delete(root);
	}
	return -1;
}
int get_config()
{
	int len = 0;//Record the length of the license value
	PUBLICPAIRKEY pairKey;
	char lic_clearText[maxn] = {0};
	char des_clearText[DES_KEY_TEXT_LENGTH + 1] = {0};
	char *licenseValue;
	int *deskeyValue;

	//initial pairKey
	pairKey.n = 0;
	pairKey.publicKey = 0;
	licenseValue = (char *)malloc(MAX_CIPHER_TEXT_LENGTH);    /* License cipher text storage */
	if(licenseValue == NULL) goto free_done;
	deskeyValue = (int *)malloc(MAX_CLEAR_TEXT_LENGTH);    /* des key cipher text storage */
	if(deskeyValue == NULL) goto free_done;

	/* Read license value */
	len = readLicenseValue((unsigned char *)licenseValue, deskeyValue, &pairKey);
	if(0 == len) goto free_done;

	/* Decrypt file content */
	rsa_decrypt(pairKey.n, pairKey.publicKey, deskeyValue, len, des_clearText);

	/* Decrypt licesne's plaintext information based on the key value */
	DES_decrypt(licenseValue,lic_clearText,des_clearText);

	/* output clear text to file */
    output(lic_clearText);
	free(deskeyValue);
	deskeyValue = NULL;
	free(licenseValue);
	licenseValue = NULL;

	return OK;
free_done:
	if (deskeyValue) free(deskeyValue);
	if (licenseValue) free(licenseValue);
	deskeyValue = NULL;
	licenseValue = NULL;

	return ERR;
	
	//////////////////////////////////////////
	int state = 0;
	//long len = 0;	
	char *data = NULL;
	cJSON *json = NULL,*item = NULL;
	int i = 0;
	int size = 0;
	get_file_data(DATA_PATH,&data);
	// 解析数据包
	json = cJSON_Parse(data);
	if(NULL != data){
		free(data);
		data = NULL;
	}
	item = cJSON_GetObjectItem(json,"version");
	item = cJSON_GetObjectItem(json,"private.pem");
	set_file_data(PRIVATE_BAK_PATH,item->valuestring);
	item = cJSON_GetObjectItem(json,"cacert.pem");
	set_file_data(CACERT_BAK_PATH,item->valuestring);
	if(NULL != json){
		cJSON_Delete(json);
		json = NULL;
	}
	return state;
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

