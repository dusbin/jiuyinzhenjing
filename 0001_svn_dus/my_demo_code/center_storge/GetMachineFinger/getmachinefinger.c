#include <stdio.h>
#include <string.h>
#include <shcrypt.h>
#include <unistd.h>
#include <signal.h>

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
int main(){
	printf("\rtest\n");
	char m[5120],f[256];
	GetMachineFinger(m,5120,f,256);
	printf("m = %s,f = %s\n",m,f);
	return 0;
}

