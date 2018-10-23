#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int proc_sys_network_info(){
	int ret = 0;
	char buff[1024] = {0};
    int index = 0;
	char device_name[1024] = {0};
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	char cmd[256] = {0};	
	int ui_totle_len = 0;
	int iServiceInfoStrLen = 0;
	int iServiceInfoStrCount = 0;
	int line_count = 0;
	int line = 0;
	char* pServiceInfo = NULL;
	char line_data[512] = {0};
	char ch = 0;
	char name[33] = {0};
	system("ifconfig | grep -w inet|awk -F':' '{print$2}'|awk -F' ' '{print$1}' >/opt/ips.txt");
    sprintf(cmd, "cat %s | grep sip | awk -F'=' '{printf$2}'", "/opt/ips.txt");
	fp1 = fopen("/opt/ips.txt","r");
	if(NULL == fp1){
		return -1;
	}
	fseek(fp1,0,SEEK_END);
	ui_totle_len = ftell(fp1);
	if(0 == ui_totle_len){
		fclose(fp1);
        return -1;
    }
	fseek(fp1,0,SEEK_SET);
    pServiceInfo = (char *)malloc(sizeof(char)*ui_totle_len+1);
	if(pServiceInfo == NULL){
		fclose(fp1);
        return -1;	
	}
	fread(pServiceInfo,ui_totle_len,1,fp1);
    pServiceInfo[ui_totle_len]='\0';
    fclose(fp1);
    fp1 = NULL;
	iServiceInfoStrLen = strlen(pServiceInfo);
	while(iServiceInfoStrLen > iServiceInfoStrCount){
		ch = pServiceInfo[iServiceInfoStrCount];
		if(ch == '\n'){
			printf("-------------------\n");
			sscanf(line_data,"%s",buff);
			system("ifconfig >/opt/device_name.txt");
			sprintf(cmd, "cat /opt/device_name.txt| grep %s |awk -F'Mask:' '{printf$2}'", buff);
			printf("IP:%s\n",buff);
			if(NULL == (fp = popen(cmd, "r")))
				return -1;
			fgets(device_name, sizeof(device_name), fp);
			printf("MASK:%s\n",device_name);
			pclose(fp);
			sprintf(cmd, "cat /opt/device_name.txt |cat -n| grep %s | awk -F' ' '{printf$1}'", buff);
			if(NULL == (fp = popen(cmd, "r")))
				return -1;
			memset(buff,0x00,sizeof(buff));
			fgets(buff, sizeof(buff), fp);
			pclose(fp);
			line=atoi(buff)-1;
			printf("line = %d\n",line);
			//printf("buff = %s\n",buff);
			sprintf(cmd, "sed -n %dp /opt/device_name.txt |awk -F' ' '{printf$1}' ", line);
			if(NULL == (fp = popen(cmd, "r")))
				return -1;
			fgets(buff, sizeof(buff), fp);
			pclose(fp);
			printf("Name:%s\n",buff);
			strcpy(name,buff);
			memset(buff,0x00,1024);	
			if(NULL == (fp = popen("ps -ef | grep dhcp | grep -v grep |wc -l | awk -F' ' '{printf$1}'", "r")))
				return -1;
			fgets(buff, sizeof(buff), fp);
			pclose(fp);
			ret = atoi(buff);
			if(0 < ret ){
				printf("DHCP:1\n");
			}else{
				printf("DHCP:0\n");
			}
			memset(buff,0x00,1024);
			if(NULL == strstr(name,"lo")){
				sprintf(cmd, "ifconfig | grep %s |awk -F' ' '{printf$NF}'", name);
				if(NULL == (fp = popen(cmd, "r")))
					return -1;
				fgets(buff, sizeof(buff), fp);
				pclose(fp);
				printf("MAC:%s\n",buff);
			}else{
				printf("MAC:00:00:00:00:00:00\n",buff);
			}
			printf("DNS:0\n");
			system("rm /opt/device_name.txt");
			memset(line_data,0x0,sizeof(line_data));
			memset(buff,0x00,sizeof(buff));
			memset(name,0x00,sizeof(name));
			index = 0;
			line_count++;
		}else{
			line_data[index] = ch;
			index++;
		}
		iServiceInfoStrCount++;
	}
	printf("-------------------\n");
	system("rm /opt/ips.txt");
	return 0;
}
int main(){
	proc_sys_network_info();
	return 0;
}