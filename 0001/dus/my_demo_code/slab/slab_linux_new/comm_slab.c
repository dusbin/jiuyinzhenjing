#include "comm_slab.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**缓冲区操作全局变量，函数指针g_uxsino_slab_ops*/
struct uxsino_slab_ops g_uxsino_slab_op = {
	.slab_malloc = slab_malloc,
	.slab_free = slab_free,
	.slab_realloc = slab_realloc,
};
void print(uxsino_slab* slab){
    mem_unit* mem_un; 
    int i = 0;
    struct list_head *pos,*n;
    for(i = 0;i < SLAB_MAX;i++){
        printf("-----------------slab->m_used_mem_objs[%d]--------------------\n",i);
        if(!list_empty(&(slab->m_used_mem_objs[i]))){
            list_for_each_safe(pos,n,&slab->m_used_mem_objs[i]){
                mem_un = container_of(pos,mem_unit,element);
                if(mem_un){
                    printf("mem_un->p = 0x%x\n",mem_un->p);
                }
            }
            printf("------------------------------------------------------------- \n");
        }else{
            printf("slab->m_used_mem_objs[%d] is empty.\n",i);
            printf("------------------------------------------------------------- \n");
        }
        printf("-----------------slab->m_free_mem_objs[%d]--------------------\n",i);
        if(!list_empty(&(slab->m_free_mem_objs[i]))){
            list_for_each_safe(pos,n,&slab->m_free_mem_objs[i]){
                mem_un = container_of(pos,mem_unit,element);
                if(mem_un){
                    printf("mem_un->p = 0x%x\n",mem_un->p);
                }
            }
            printf("------------------------------------------------------------- \n");
        }else{
            printf("slab->m_free_mem_objs[%d] is empty.\n",i);
            printf("------------------------------------------------------------- \n");
        }
    }
}
/**************************************************************    
* 函数名称：slab_create()
* 功能概要：申请空间，初始化slab
* 功能详细：对创建的slab进行初始化
* 输入参数：
* 输出参数：无
* 返回值：  uxsino_slab *
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/

uxsino_slab *slab_create(){
	uxsino_slab *slab = (uxsino_slab*)malloc(sizeof(uxsino_slab));
	int i = 0;
	int slab_max_num_array[SLAB_MAX]={1,1,1,1,8,4,2,1};
	slab->slab_max_num = slab_max_num_array[0];
	slab->m_unit_size[0] = SLAB_UNIT_SIZE;
	slab->m_unit_max_num[0] = slab_max_num_array[0];
	slab->slab_size_sum = slab->m_unit_size[0]*slab->m_unit_max_num[0];
	for(i = 1;i < SLAB_MAX;i++){
        slab->m_unit_size[i] = slab->m_unit_size[i-1]*2;
        slab->m_unit_max_num[i] = slab_max_num_array[i];
        slab->slab_size_sum += slab->m_unit_size[i]*slab->m_unit_max_num[i];
        slab->slab_max_num += slab_max_num_array[i];
        //printf("slab->m_unit_size[%d] = %d\n",i,slab->m_unit_size[i]);
	}
	slab->m_mem_uns = (mem_unit*)malloc(sizeof(mem_unit)*(slab->slab_max_num));
	if(slab->m_mem_uns == NULL)
        return (uxsino_slab *)NULL;
    //printf("slab->m_unit_size_sum = %d slab->slab_max_num = %d\n",slab->slab_size_sum,slab->slab_max_num);
	slab->m_mem_unit = (void*)malloc(slab->slab_size_sum);
	//printf("slab->m_mem_unit 0x%x\n",slab->m_mem_unit);
	if(slab->m_mem_unit == NULL){
        free(slab->m_mem_uns);
        slab->m_mem_uns = NULL;
        return (uxsino_slab *)NULL;
	}
	slab->ops = &g_uxsino_slab_op;
    memset(slab->m_mem_unit,0x00,slab->slab_size_sum);
    //printf("slab->m_unit_size_sum = %d\n",slab->slab_size_sum);
    for(i = 0;i < SLAB_MAX;i++){
	    INIT_LIST_HEAD(&(slab->m_free_mem_objs[i]));
	    INIT_LIST_HEAD(&(slab->m_used_mem_objs[i]));
	}
	int j = 0;
	int count = 0;
	int offset = 0;
	for(i = 0;i < slab->slab_max_num;i++){
	    if(j < SLAB_MAX){
	        if(count < slab->m_unit_max_num[j]){
                slab->m_mem_uns[i].p = (void*)(slab->m_mem_unit + offset);
                //printf("slab->m_mem_uns[%d].p 0x%x   %d\n", i,slab->m_mem_uns[i].p,offset);
                list_add_tail(&(slab->m_mem_uns[i]).element,&(slab->m_free_mem_objs[j]));
                offset += slab->m_unit_size[j];
                count++;
            }else{
                count = 0;
                j++;
                slab->m_mem_uns[i].p = (void*)(slab->m_mem_unit + offset);
                //printf("slab->m_mem_uns[%d].p 0x%x   %d\n",i  ,slab->m_mem_uns[i].p,offset);
                list_add_tail(&(slab->m_mem_uns[i]).element,&(slab->m_free_mem_objs[j]));
                offset += slab->m_unit_size[j];
                count++;
            }
        }
	}
	//print(slab);
	return slab;
}
/**************************************************************    
* 函数名称：slab_release()
* 功能概要：使用完毕，释放申请的空间
* 功能详细：使用完毕，释放申请的空间
* 输入参数：uxsino_slab* slab(需要释放的uxsino_slab*)
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void slab_release(uxsino_slab* slab){
	if(slab->m_mem_uns != NULL){
        free(slab->m_mem_uns);
        slab->m_mem_uns = NULL;
	}
	if(slab->m_mem_unit != NULL){
        free(slab->m_mem_unit);
        slab->m_mem_unit = NULL;
	}
	if(slab != NULL){
        free(slab);
        slab=NULL;
	}
	return;
}
/**************************************************************    
* 函数名称：slab_malloc()
* 功能概要：申请slab缓冲区内存
* 功能详细：申请slab缓冲区内存
* 输入参数：uxsino_slab* slab(slab_malloc缓冲区的slab变量)，malloc_size(申请的大小)
* 输出参数：无
* 返回值：  void*
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void* slab_malloc(uxsino_slab* slab,int malloc_size){
	mem_unit* mem_un;
	int i = 0; 
	for(i = 0;i < SLAB_MAX;i++){
	    if((malloc_size <= slab->m_unit_size[i])&&(!list_empty(&(slab->m_free_mem_objs[i])))){
	        mem_un = container_of(slab->m_free_mem_objs[i].next,mem_unit,element);
            list_del(&mem_un->element);
            list_add_tail(&mem_un->element,&(slab->m_used_mem_objs[i]));
            //printf("mem_un->p = 0x%x\n",mem_un->p);
            memset(mem_un->p,0x00,slab->m_unit_size[i]);
            //printf("slab_malloc,i = %d,size = %d\n",i,malloc_size);
            //print(slab);
            return (void*)(mem_un->p);
        }else{
            //printf("slab->m_unit_size[%d] = %d\n",i,slab->m_unit_size[i]);
        }
	}
	//print(slab);
	//printf("malloc,%d\n",malloc_size);
    return (void*)malloc(malloc_size);
}
/**************************************************************    
* 函数名称：is_in_slab()
* 功能概要：判断指针是否在slab缓冲区里
* 功能详细：判断指针是否在slab缓冲区里
* 输入参数：uxsino_slab* slab(slab_malloc缓冲区的slab变量)，const void* ptr(需要判断的指针)
* 输出参数：无
* 返回值：  int -1 未找到，>=0 返回所在的数组编号
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
int is_in_slab(uxsino_slab* slab,const void* ptr)//判断存在于缓冲区内
{
    if (ptr < slab->m_mem_unit || ptr > slab->m_mem_unit + slab->slab_size_sum) {
        return -1;
    }
    int i = 0;
    void* ptr_tmp;
    ptr_tmp = slab->m_mem_unit + slab->m_unit_max_num[0]*slab->m_unit_size[0];
    if(ptr >= slab->m_mem_unit && ptr < ptr_tmp){
        return 0;
    }
    for(i = 1; i < SLAB_MAX;i++){
        if(ptr >= ptr_tmp && ptr <ptr_tmp + slab->m_unit_max_num[i]*slab->m_unit_size[i])
        {
            return i;
        }else{
            ptr_tmp += slab->m_unit_max_num[i]*slab->m_unit_size[i];
        }
    }
    return -1;
}
/**************************************************************    
* 函数名称：slab_free()
* 功能概要：回收slab缓冲区内存
* 功能详细：回收slab缓冲区内存(回收完毕后需要自行将ptr的指针指向NULL)
* 输入参数：uxsino_slab* slab(slab_malloc缓冲区的slab变量)，void * ptr(需要回收的内存地址)
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void slab_free(uxsino_slab* slab,void * ptr){
	mem_unit* mem_un;
	int i = is_in_slab(slab,ptr);
	if(i >= 0 && i < SLAB_MAX){
        mem_un = container_of(slab->m_used_mem_objs[i].next,mem_unit,element);
        memset(mem_un->p,0x00,slab->m_unit_size[i]);
        list_del(&mem_un->element);
        list_add_tail(&mem_un->element,&(slab->m_free_mem_objs[i]));
	}else{
        free(ptr);
	}
	//print(slab);
	return;
}
/**************************************************************    
* 函数名称：slab_realloc()
* 功能概要：扩大内存申请slab缓冲区内存
* 功能详细：扩大内存slab缓冲区内存
* 输入参数：uxsino_slab* slab(slab_malloc缓冲区的slab变量)，ptr 原指针，malloc_size(申请的大小)
* 输出参数：无
* 返回值：  void*
* 创建日期：2017/12/08    
* 创建作者：duzhengbin
* **************************************************************/
void* slab_realloc(uxsino_slab* slab,void* ptr,int malloc_size){
    int flag = is_in_slab(slab,ptr);
    if(malloc_size <= slab->m_unit_size[flag]){
        return ptr;
    }
    void* newptr = slab->ops->slab_malloc(slab,malloc_size);
    memcpy(newptr,ptr,slab->m_unit_size[flag]);
    slab->ops->slab_free(slab,ptr);
    return newptr;
}

