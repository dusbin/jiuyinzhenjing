#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
	int i = 1;
	while(i<=100){
		if(i%2 == 0){
			system("echo \" -.-\"");
			system("echo \"  -\"");
		}else{
			system("echo \" 0.0\"");
			system("echo \"  -\"");
		}
		sleep(1);
		system("clear");
		i++;
	}
	return 0;
}

