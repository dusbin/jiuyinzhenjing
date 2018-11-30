#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <errno.h>

void help(void)
{
    printf("用户密码验证程序\n第一个参数为用户名!\n");
    exit(-1);
}
void error_quit(char *msg)
{
    perror(msg);
    exit(-2);
}
void get_salt(char *salt,char *passwd)
{
    int i,j;


    //取出salt,i记录密码字符下标,j记录$出现次数
    for(i=0,j=0;passwd[i] && j != 3;++i)
    {
        if(passwd[i] == '$')
            ++j;
    }


    strncpy(salt,passwd,i-1);
}
int main(int argc,char **argv)
{
    struct spwd *sp;
    char *passwd;
    char salt[512]={0};


    if(argc != 2)
        help();


    //输入用户名密码
    passwd=getpass("请输入密码:");
    //得到用户名以及密码,命令行参数的第一个参数为用户名
    if((sp=getspnam(argv[1])) == NULL)
        error_quit("获取用户名和密码");
    //得到salt,用得到的密码作参数
    get_salt(salt,sp->sp_pwdp);


    //进行密码验证
	char *pass = crypt(passwd,salt);
    if(strcmp(sp->sp_pwdp,pass) == 0)
        printf("验证通过!\n");
    else
        printf("验证失败!\n");


    return 0;
}

