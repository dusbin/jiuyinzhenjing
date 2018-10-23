#include<stdio.h>
#include<stdlib.h>
struct _tag_mem_unit{
	const char* ptr;
	struct mem_unit* element;
}mem_unit;
int main(){
	printf("test begin...\n");
	void* dzb = malloc(1024);
	struct mem_unit * mem;
	mem->ptr = dzb;
	mem->element = dzb+ 4;
	printf("test end...\n");
	return 0;
}

