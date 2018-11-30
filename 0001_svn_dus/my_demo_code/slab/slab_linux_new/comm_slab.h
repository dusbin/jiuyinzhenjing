#ifndef _COMM_SLAB_H_
#define _COMM_SLAB_H_
#include "simple_list.h"
#include <stdio.h>
#define SLAB_MAX 8                                       /**<缓冲区块种类最大值*/
#define SLAB_UNIT_SIZE 8                                /**<缓冲区块最小单元*/        
struct uxsino_slab_ops;
struct uxsino_slab;
/**缓冲区块结构*/
typedef struct _tag_mem_unit{
	struct list_head element;                           /**<缓冲区内存链表结构*/
    void* p;                                             /**<指向缓冲区内存区域的指针*/
}mem_unit;
/**缓冲区结构体*/
typedef struct _tag_uxsino_slab{
    size_t m_unit_max_num[SLAB_MAX];                   /**<缓冲区块数*/
    size_t m_unit_size[SLAB_MAX];                      /**<缓冲区大小*/
    size_t slab_size_sum;                               /**<缓冲区总大小*/
    size_t slab_max_num;                                /**<缓冲区总块数*/
    mem_unit* m_mem_uns;                                /**<缓冲区块结构*/
    struct list_head m_free_mem_objs[SLAB_MAX];       /**<剩余空间列表*/
    struct list_head m_used_mem_objs[SLAB_MAX];       /**<已用空间列表*/
    struct uxsino_slab_ops* ops;                        /**<缓冲区操作结构体*/
    void* m_mem_unit;                                   /**<缓冲区内存区域*/
}uxsino_slab;
/**缓冲区操作结构体*/
struct uxsino_slab_ops{
    void* (*slab_malloc)(uxsino_slab* slab,int malloc_size);
    void  (*slab_free)(uxsino_slab* slab,void* ptr);
    void* (*slab_realloc)(uxsino_slab* slab,void* ptr,int malloc_size);
};
uxsino_slab* slab_create();
void slab_release(uxsino_slab* slab);
void* slab_malloc(uxsino_slab* slab,int malloc_size);
void slab_free(uxsino_slab* slab,void* ptr);
void* slab_realloc(uxsino_slab* slab,void* ptr,int malloc_size);
#endif

