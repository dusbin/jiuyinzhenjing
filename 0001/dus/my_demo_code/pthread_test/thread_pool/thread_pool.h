#ifndef __THREAD_POOL_H_INC__
#define __THREAD_POOL_H_INC__
#include <stdio.h>
#include "types.h"
#include "Simplelist.h"
#include <pthread.h>
#define FUNC_ERR -1
#define FUNC_OK 0
typedef void*(*thread_pool_callback_func)(void* arg);
struct thread_pool {
    int thread_num;             /**< 线程数*/
    
    pthread_mutex_t task_lock;  /**< 任务队列锁*/
    struct list_head tasks;     /**< 任务队列*/
    int task_num;               /**< 任务队列中的任务数*/
    
    pthread_mutex_t curr_running_task_num_lock;      /**<  正在处理的任务数锁*/
    int curr_running_task_num;           /**< 正在处理的任务数*/
    
    pthread_t* threads;     /**< 线程ID*/
    int* started;          /**< */
    BOOL exit;              /**< 线程池退出标志*/

    pthread_mutex_t task_cond_lock;     /**< 任务条件变量锁*/
    pthread_cond_t task_cond;           /**< 任务条件变量*/
};
struct thread_pool* thread_pool_create(int thread_num);
void thread_pool_release(struct thread_pool* pool);
int thread_pool_add_task(struct thread_pool* pool, thread_pool_callback_func cb, void* arg);
#endif

