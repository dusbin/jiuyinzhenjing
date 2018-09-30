//https://www.cnblogs.com/orlion/p/6258691.html
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
	char buf[5];
	char buf2[10];
	fgets(buf, 5, stdin); //第一次输入时,超过5个字符
	puts(stdin->_IO_read_ptr);//本句说明整行会被一次全部读入缓冲区,
	//而非仅仅上面需要的个字符
	stdin->_IO_read_ptr = stdin->_IO_read_end; //标准I/O会认为缓冲区已空,再次调用read
	//注释掉,再看看效果
	printf("\n");
	puts(buf);
	fgets(buf2, 10, stdin);
	puts(buf2);
	return 0;
}

