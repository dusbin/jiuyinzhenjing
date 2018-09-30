//https://www.cnblogs.com/orlion/p/6258691.html
/*
 
before reading
read buffer base (nil)
read buffer length 0
write buffer base (nil)
write buffer length 0
buf buffer base (nil)
buf buffer length 0
读操作前myfile的缓冲区是没有被分配的，一次读之后
myfile的缓冲区才被分配.这个缓冲区既不是内核中的缓冲
区,也不是用户分配的缓冲区,而是有用户进程空间中的由
buffered I/O系统负责维护的缓冲区
1234567
after reading
read buffer base 0x7f9fed030000
read buffer length 8
write buffer base 0x7f9fed030000
write buffer length 0
buf buffer base 0x7f9fed030000
buf buffer length 1024
1234
 
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
	char buf[5];
	FILE *myfile = stdin;
	printf("before reading\n");
    printf("read buffer base %p\n", myfile->_IO_read_base);
    printf("read buffer length %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
    printf("write buffer base %p\n", myfile->_IO_write_base);
    printf("write buffer length %d\n", myfile->_IO_write_end - myfile->_IO_write_base);
    printf("buf buffer base %p\n", myfile->_IO_buf_base);
    printf("buf buffer length %d\n", myfile->_IO_buf_end - myfile->_IO_buf_base);
    printf("\n");
	fgets(buf,5,myfile);
	fputs(buf,myfile);
	printf("after reading\n");
    printf("read buffer base %p\n", myfile->_IO_read_base);
    printf("read buffer length %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
    printf("write buffer base %p\n", myfile->_IO_write_base);
    printf("write buffer length %d\n", myfile->_IO_write_end - myfile->_IO_write_base);
    printf("buf buffer base %p\n", myfile->_IO_buf_base);
    printf("buf buffer length %d\n", myfile->_IO_buf_end - myfile->_IO_buf_base);
	//buf[5] = '\0';
	printf("%s",buf);
	return 0;
}

