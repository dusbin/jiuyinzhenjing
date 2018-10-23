#ifndef _COMM_SLAB_H_
#define _COMM_SLAB_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/**缓冲区块结构*/
typedef struct _tag_mem_unit{
    void * p;
    struct _tag_mem_unit* next;
}mem_unit;
struct uxsino_slab_ops;
struct uxsino_slab;
/**缓冲区结构体*/
typedef struct _tag_uxsino_slab{
    size_t m_unit_max_num;/**<缓冲区块数,创建时由调用者输入*/
    size_t m_unit_size;/**<缓冲区块数,创建时由调用者输入*/
    mem_unit* m_mem_uns;/**<缓冲区块结构*/
    struct _tag_mem_unit* m_free_mem_objs;/**<剩余空间列表*/
    struct _tag_mem_unit* m_used_mem_objs;/**<已用空间列表*/
    struct uxsino_slab_ops* ops;/**<缓冲区操作结构体*/
    void* m_mem_unit;/**<缓冲区内存区域*/
}uxsino_slab;
int create_slab(uxsino_slab* slab,size_t max_num,size_t size);
int release_slab(uxsino_slab* slab);
void* alloc_slab(uxsino_slab* slab,size_t malloc_size);
void free_slab(uxsino_slab* slab,void* ptr);
/**缓冲区操作结构体*/
struct uxsino_slab_ops{
    int (*create_slab)(uxsino_slab* slab,size_t max_num,size_t size);
    int (*release_slab)(uxsino_slab* slab);
    void* (*alloc_slab)(uxsino_slab* slab,size_t malloc_size);
    void (*free_slab)(uxsino_slab* slab,void* ptr);
};
/**缓冲区操作全局变量，函数指针g_uxsino_slab_ops*/
struct uxsino_slab_ops g_uxsino_slab_ops = {
    .create_slab = create_slab,
    .release_slab = release_slab,
    .alloc_slab = alloc_slab,
    .free_slab = free_slab,
};
/**************************************************************		
* 函数名称：create_slab()
* 功能概要：对uxsino_slab申请内存，并对uxsino_slab初始化
* 功能详细：对uxsino_slab申请内存，并对uxsino_slab初始化
* 输入参数：uxsino_slab* slab,size_t max_num,size_t size
* 输出参数：无
* 返回值：  0:成功；-1:失败
* 创建日期：2017/10/19		
* 创建作者：duzhengbin
* **************************************************************/
int create_slab(uxsino_slab* slab,size_t max_num,size_t size ){
    slab->m_unit_size = size;
    slab->m_unit_max_num = max_num;
    slab->m_mem_unit = NULL;
    slab->m_mem_uns = NULL;
    slab->m_mem_unit = (void*)malloc(max_num*size);
    if(slab->m_mem_unit == NULL)return -1;
    memset(slab->m_mem_unit,0x00,max_num*size);
    slab->m_mem_uns = (mem_unit*)malloc(sizeof(mem_unit)*max_num);
    if(slab->m_mem_uns == NULL){
        free(slab->m_mem_unit);
        slab->m_mem_unit = NULL;
        return -1;
    }
    int i = 0;
    for(i = 0;i < slab->m_unit_max_num;i++){
        slab->m_mem_uns[i].p = NULL;
        slab->m_mem_uns[i].next = NULL;
    }
    for(i = 0;i < slab->m_unit_max_num;i++){
        (slab->m_mem_uns[i].p) = (void*)(slab->m_mem_unit+i*slab->m_unit_size);
        slab->m_mem_uns[i].next = &slab->m_mem_uns[i+1];
    }
    slab->m_mem_uns[slab->m_unit_max_num - 1].next = NULL;
    slab->m_free_mem_objs = &slab->m_mem_uns[0];
    slab->m_used_mem_objs = NULL;
    return 0;
}
/**************************************************************		
* 函数名称：release_slab()
* 功能概要：释放uxsino_slab申请的内存
* 功能详细：释放uxsino_slab申请的内存
* 输入参数：uxsino_slab* slab
* 输出参数：无
* 返回值：  0:成功；-1:失败
* 创建日期：2017/10/19		
* 创建作者：duzhengbin
* **************************************************************/
int release_slab(uxsino_slab* slab){
    free(slab->m_mem_unit);
    free(slab->m_mem_uns);
    slab->m_mem_unit = NULL;
    slab->m_mem_uns = NULL;
    slab->m_free_mem_objs = NULL;
    slab->m_used_mem_objs = NULL;
    return 0;
}
/**************************************************************		
* 函数名称：alloc_slab()
* 功能概要：使用uxsino_slab申请内存
* 功能详细：使用uxsino_slab申请内存
* 输入参数：uxsino_slab* slab,size_t malloc_size
* 输出参数：无
* 返回值：  void*
* 创建日期：2017/10/19		
* 创建作者：duzhengbin
* **************************************************************/
void* alloc_slab(uxsino_slab* slab,size_t malloc_size){
    if(malloc_size > slab->m_unit_size){
        //printf("malloc by self\n");
        return malloc(malloc_size);
    }
    if(NULL == slab->m_free_mem_objs){
        //printf("malloc by self\n");
        return malloc(slab->m_unit_size);
    }
    struct _tag_mem_unit* free = slab->m_free_mem_objs;
    slab->m_free_mem_objs = slab->m_free_mem_objs->next;
    free->next = slab->m_used_mem_objs;
    slab->m_used_mem_objs = free;
    //printf("malloc by slab\n");
    return (free->p);
}
/**************************************************************		
* 函数名称：free_slab()
* 功能概要：使用uxsino_slab释放申请的内存
* 功能详细：使用uxsino_slab释放申请的内存，释放完内存后需要将ptr指针指向NULL
* 输入参数：uxsino_slab* slab,void* ptr
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/19		
* 创建作者：duzhengbin
* **************************************************************/

void free_slab(uxsino_slab* slab,void* ptr){
    if(ptr == NULL){
        return;
    }
    if(NULL == slab->m_used_mem_objs){
        if(ptr != NULL){
            //printf("free by self\n");
            free(ptr);
        }
        return;
    }
    struct _tag_mem_unit* temp = slab->m_used_mem_objs;
    if((temp->p) == ptr){
        memset(ptr,0x00,slab->m_unit_size);
        slab->m_used_mem_objs = slab->m_used_mem_objs->next;
        temp->next = slab->m_free_mem_objs;
        slab->m_free_mem_objs = temp;
        //printf("free by slab\n");
        return;
    }
    struct _tag_mem_unit* temp1 = temp->next;
    do{
        if((temp1->p) == ptr){
            memset(ptr,0x00,slab->m_unit_size);
            temp->next = temp->next->next;
            temp1->next = slab->m_free_mem_objs;
            slab->m_free_mem_objs = temp1;
            //printf("free by slab\n");
            return;
        }
        temp = temp->next;
        temp1 = temp->next;
    }while(temp1 != NULL);
    if(ptr != NULL){
        free(ptr);
        //printf("free by self\n");
    }
    return;
}
#endif
