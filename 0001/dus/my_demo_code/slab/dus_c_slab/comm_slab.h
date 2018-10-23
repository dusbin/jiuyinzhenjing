#ifndef _COMM_SLAB_H_
#define _COMM_SLAB_H_

#include <assert.h>
#include "simple_list.h"
#define ERROR_MALLOC_FAILED -1
/**缓冲区块结构*/
struct _tag_mem_unit{
	void* ptr;
	struct list_head element;
}mem_unit;
/**缓冲区操作(函数)结构*/
struct uxsino_slab_ops{
	int(*create)(uxsino_slab slab,int slab_max_num,int slab_size);
	int(*release)(uxsino_slab slab);
	bool(*is_in_slab)(const void* ptr);
	void*(*alloc)();
	void*(*free)(void* pptr);
}
/**缓冲区结构*/
struct _tag_uxsino_slab{
	int m_unit_max_num;/*缓冲区块数,创建时由调用者输入*/
	int m_unit_size;/*缓冲区块数*/
	void* m_mem_unit;
	struct mem_unit* m_mem_uns;
	struct list_head m_free_mem_objs;/*free*/
    struct list_head m_used_mem_objs;/*used*/
	struct uxsino_slab_ops* ops;/*缓冲区块操作*/
}uxsino_slab;
/**函数*/
int create_slab(struct uxsino_slab* slab,int slab_max_num,int slab_size);
int release_slab(struct uxsino_slab* slab);
bool is_in_slab(const void* ptr);
void* alloc();
void free(void* pptr);
/**全局变量g_uxsino_slab_ops*/
struct uxsino_slab_ops g_uxsino_slab_ops = {
	.create = create_slab,
	.release = release_slab,
	.is_in_slab = is_in_slab,
	.alloc = alloc,
	.free = free,
};
/*
 *初始化slab
 *参数slab [ops] max size 自身，块数，块大小
 */
int INIT_SLAB(struct uxsino_slab* slab/*,struct uxsino_slab_ops* ops*/,int max,int size){
	assert(slab&&g_uxsino_slab_ops);
	slab->ops = g_uxsino_slab_ops;
	slab.ops->create(slab,max,size);//创建，参数为自身，块数，块大小
	return 0;
}




