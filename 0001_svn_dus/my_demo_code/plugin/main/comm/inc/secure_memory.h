#ifndef __SECURE_MEMORY_H__
#define __SECURE_MEMORY_H__
#include "simple_list.h"
#include "comm_define.h"
typedef struct memory_unit{
	struct list_head element;
	void* p;
	long time;
}memory_unit;
void* malloc_s(unsigned size);
void free_s(void *p);
int monitor_memory();
int memory_test();
#endif

