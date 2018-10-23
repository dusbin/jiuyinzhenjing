#include<stdio.h>
#include<stdlib.h>
#include "../db/db_interface.h"
#include "../db/sqlite3.h"
#include "../ux_comm_lib/uxmd5.h"
#include <dirent.h>
#include <sys/stat.h>
#include<unistd.h>
#include<string.h>
#define PATH_LEN 1024*2
#define MD5_LEN 33
#define MAX 100
#define CMD_LEN 1024*2
#define SQL_LEN 1024*10
DB_CB_ST *db_inst = NULL;
void List(char *path);
int findmd5fromdb(const char *file_md5,const char *filepath,const char *filename);
void addtodb(const char * file_md5,const char * filepath,const char * filename);
void printtext(char *textview);
int numall = 0;
int count = 0;
char nothing[10]="          ";//防止刷新混乱。不可删除
char textview[PATH_LEN];
int len = 0;
int main(){
	char path[PATH_LEN];
	printf("begin\n");
	int ret = -1;
	int i = 0;
	FILE *fp=NULL;
	char cmd[PATH_LEN];
	char buff[10];
	static char create_table_str[] = "create table if not exists db_file_md5(md5 varchar(33),file_path varchar(65535),file_name varchar(65535));";
    static char drop_table_str[] = "drop table if exists db_file_md5;";
	static char create_clean_table_str[] = "create table if not exists clean_db(file varchar(65535),clean_file varchar(65535),file_name varchar(65535));";
    static char drop_clean_table_str[] = "drop table if exists clean_db;";
	char insert_table_str[SQL_LEN] = {0};
	//connect db
	if(0 != db_init()){
		printf("database init err. \n");
		return -1;
	}
	db_inst = get_db_instance();
	if (!db_inst){
		printf("database get db_inst err. \n");
		goto END_ERR;
	}
	ret = db_inst->db_exec_fun("begin;");
    if (ret) goto END_ERR;
    ret = db_inst->db_exec_fun(drop_table_str);
    if (ret){
        db_inst->db_exec_fun("rollback;");
        goto END_ERR;
    }
	ret = db_inst->db_exec_fun("commit;");
    if (ret) goto END_ERR;
	ret = db_inst->db_exec_fun(create_table_str);
	if (ret) goto END_ERR;
		ret = db_inst->db_exec_fun("begin;");
    if (ret) goto END_ERR;
    ret = db_inst->db_exec_fun(drop_clean_table_str);
    if (ret){
        db_inst->db_exec_fun("rollback;");
        goto END_ERR;
    }
	ret = db_inst->db_exec_fun("commit;");
    if (ret) goto END_ERR;
	ret = db_inst->db_exec_fun(create_clean_table_str);
	if (ret) goto END_ERR;
	printf("INPUT clean path\n");
	scanf("%s",path);
	DIR *pDir=NULL;
    pDir=opendir(path);
	if(NULL == pDir){
		printf("path is error,please make sure.\n");
		return -1;
	}
	sprintf(cmd,"ls -laR \"%s\"|grep \"^-\"|wc -l|awk -F ' ' '{printf$1}'",path);
	fp=popen(cmd,"r");
	if(fp == NULL){
		return -1;
	}
	fgets(buff,sizeof(buff),fp);
	pclose(fp);
	fp = NULL;
	numall=atoi(buff);
	List(path);
	printtext("end");
	printf("\n");
END_ERR:
	db_close();
	return 0;
}
void List(char *path)
{
    struct dirent* ent = NULL;
    DIR *pDir=NULL;
    pDir=opendir(path);
	if(NULL == pDir){
		return;
	}
	char filename[PATH_LEN] = {0};
	char file_md5[MD5_LEN] = {0};
	char cmd[CMD_LEN] = {0};
    while (NULL != (ent=readdir(pDir)))
    {
		if (ent->d_type==8)
		{
			memset(file_md5,0x00,sizeof(file_md5));
			memset(filename,0x00,sizeof(filename));
			strcpy(filename,path);
			strcat(filename,"/");
			strcat(filename,ent->d_name);
			ux_md5sum(filename,file_md5);
			//printf("普通文件:[%s][%s]\n", file_md5,ent->d_name);
			if(1==findmd5fromdb(file_md5,filename,ent->d_name)){
				//rm -rf filename
				sprintf(cmd,"rm -rf \"%s\"",filename);
				count++;
				memset(textview,0x00,sizeof(textview));
				sprintf(textview,"[%d/%d][%d%%] clean",count,numall,count*100/numall);
				printtext(textview);
				//fflush(stdout);
				system(cmd);
			}else{
				//add to db
				addtodb(file_md5,filename,ent->d_name);
				count++;
				memset(textview,0x00,sizeof(textview));
				sprintf(textview,"[%d/%d][%d%%] add",count,numall,count*100/numall);
				printtext(textview);
			}
		}
		else if(ent->d_type != 10)
		{
			printtext("");
			if(0==strcmp(ent->d_name,".")||0==strcmp(ent->d_name,"..")){
			}else{
				memset(filename,0x00,sizeof(filename));
				strcpy(filename,path);
				strcat(filename,"/");
				strcat(filename,ent->d_name);
				List(filename);
			}
		}else{
			printtext("");
		}
    }
}
int findmd5fromdb(const char *file_md5,const char *filepath,const char *filename){
	char sql[SQL_LEN] = {0};
	int len = 0;
	char **data = NULL;
	char buff[PATH_LEN] = {0};
	int ret = -1;
	int row = 0,col = 0;
	sprintf(sql,"select * from db_file_md5 where md5 = '%s'",file_md5);
	db_inst->db_query_fun(sql,&data,&row,&col);
	if(row == 0){
		db_inst->db_free_query_result_fun(data);
		return 0;//not find
	}
	len = strlen(data[col+1]);
	memcpy(buff,data[col+1],len);
	db_inst->db_free_query_result_fun(data);
	memset(sql,0x00,sizeof(sql));
	ret = db_inst->db_exec_fun("begin;");
	sprintf(sql, "insert into clean_db values('%s','%s','%s');",buff,filepath,filename);
	ret = db_inst->db_exec_fun(sql);
	ret = db_inst->db_exec_fun("commit;");	
	return 1;//not find
}
void addtodb(const char * file_md5,const char * filepath,const char * filename){
	int ret = -1;
	char sql[SQL_LEN] = {0};
	ret = db_inst->db_exec_fun("begin;");
	sprintf(sql, "insert into db_file_md5 values('%s','%s','%s');",file_md5,filepath,filename);
	ret = db_inst->db_exec_fun(sql);
	ret = db_inst->db_exec_fun("commit;");
}
void printtext(char *textview){
	int len_i = 0;
	int i = 0;
	len_i=strlen(textview);
	int max = 0;
	if(len_i > MAX){
		printf("%.*s",MAX,textview);
		len_i = MAX;
	}else{
		printf("%s",textview);
	}
	
	if(len_i<len){
		max = (len-len_i+10)/10;
		for(i = 0;i < max;i++){
			printf("%s",nothing);
		}
	}else{
		len = len_i;
	}
	printf("\r");
	fflush(stdout);
}

