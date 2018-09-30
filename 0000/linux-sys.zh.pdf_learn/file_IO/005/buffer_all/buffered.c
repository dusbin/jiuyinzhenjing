//https://www.cnblogs.com/orlion/p/6258691.html
/*
 
上面提到的bbb.txt文件的内容是由很多行的"123456789"组成上例中,fgets(buf, 5, myfile); 仅仅读4个字符,但是,缓冲区已被写满,但是_IO_read_ptr却向前移动了5位,下次再次调用读操作时,只要要读的位数不超过myfile->_IO_read_end - myfile->_IO_read_ptr那么就不需要再次调用系统调用read,只要将数据从myfile的缓冲区拷贝到buf即可(从myfile->_IO_read_ptr开始拷贝)
　　全缓冲读的时候,_IO_read_base始终指向缓冲区的开始，_IO_read_end始终指向已从内核读入缓冲区的字符的下一个(对全缓冲来说,buffered I/O读每次都试图都将缓冲区读满)，IO_read_ptr始终指向缓冲区中已被用户读走的字符的下一个(_IO_read_end < (_IO_buf_base-_IO_buf_end)) && (_IO_read_ptr == _IO_read_end)时则已经到达文件末尾其中_IO_buf_base-_IO_buf_end是缓冲区的长度
　　一般大体的工作情景为:第一次fgets(或其他的)时,标准I/O会调用read将缓冲区充满,下一次fgets不调用read而是直接从该缓冲区中拷贝数据,直到缓冲区的中剩余的数据不够时,再次调用read.在这个过程中,_IO_read_ptr就是用来记录缓冲区中哪些数据是已读的,
哪些数据是未读的.
 
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
	char buf[5];
	char *cur;
	FILE *myfile;
	myfile = fopen("bbb.txt", "r");
	printf("before reading, myfile->_IO_read_ptr: %d\n", myfile->_IO_read_ptr - myfile->_IO_read_base);
	fgets(buf, 5, myfile); //仅仅读4个字符
	cur = myfile->_IO_read_base;
	while (cur < myfile->_IO_read_end) //实际上读满了这个缓冲区
	{
		printf("%c",*cur);
		cur++;
	}
	printf("\nafter reading, myfile->_IO_read_ptr: %d\n", myfile->_IO_read_ptr - myfile->_IO_read_base);
	return 0;
}

