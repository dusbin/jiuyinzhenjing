/*
 * Author: Herbert <wang_xiaoq@126.com>
 * License: GPL v2
 * */

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "thread-pool.h"

#define NO_DEBUG

#ifndef NO_DEBUG

#define DEBUG(s) do { \
    fprintf(stderr, "[file: %s, line: %d, function: %s]: %s\n", __FILE__, __LINE__, __FUNCTION__, s); \
} while(0)

#else

#define DEBUG(s)

#endif

enum {
    THREAD_POOL_ACTIVE = 0,
    THREAD_POOL_DESTROYING,
};

typedef struct job {
    job_func_t job_func;
    void *arg;
    struct job *next;
} job_t;

typedef struct job_queue {
    pthread_mutex_t job_queue_mutex;
    pthread_cond_t job_queue_cond;
    job_t *head;
    job_t *tail;
    int job_num;
} job_queue_t;

typedef struct thread_struct {
    pthread_t tid;
    struct thread_struct *next;
} thread_struct_t;

typedef struct thread_queue {
    pthread_mutex_t thread_queue_mutex;
    thread_struct_t *head;
    thread_struct_t *tail;
    int thread_num;
} thread_queue_t;

/*
 * thread pool data structure, singleton pattern.
 */
typedef struct thread_pool {
    thread_queue_t thread_queue;
    job_queue_t job_queue;
    pthread_rwlock_t status_lock;
    int status;
} thread_pool_t;

/* 
 * global data structure, singleton pattern
 */
PRIVATE thread_pool_t thread_pool;

/*********************** job_queue methods *********************/

PRIVATE inline job_queue_t *get_job_queue(void)
{
    return &(thread_pool.job_queue);
}

PRIVATE inline void job_queue_init(void)
{
    job_queue_t *job_queue = get_job_queue();

    pthread_mutex_init(&(job_queue->job_queue_mutex), NULL);
    pthread_cond_init(&(job_queue->job_queue_cond), NULL);
    job_queue->head = NULL;
    job_queue->tail = NULL;
    job_queue->job_num = 0;
}

/*
 * @wait: 0: destroy the job queue immediately.
 *        1: wait all the job on the job queue to complete.
 */
PRIVATE inline void job_queue_destroy(int wait)
{
    job_queue_t *job_queue = get_job_queue();
    job_t *job = NULL;
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = 500 * 1000;

    if (wait == 0) {
        pthread_mutex_lock(&(job_queue->job_queue_mutex));
        while (job_queue->head) {
            job = job_queue->head;
            job_queue->head = job->next;
            free(job);
            job = NULL;
            job_queue->job_num--;
        }
        job_queue->job_num = -1;
        pthread_mutex_unlock(&(job_queue->job_queue_mutex));
        pthread_cond_broadcast(&(job_queue->job_queue_cond));
    } else {
        pthread_mutex_lock(&(job_queue->job_queue_mutex));
        while (job_queue->job_num > 0) {
            pthread_mutex_unlock(&(job_queue->job_queue_mutex));
            select(0, NULL, NULL, NULL, &delay);
            pthread_mutex_lock(&(job_queue->job_queue_mutex));
        }
        job_queue->job_num = -1;
        pthread_mutex_unlock(&(job_queue->job_queue_mutex));
        pthread_cond_broadcast(&(job_queue->job_queue_cond));
    }
}

/* 
 * job_queue->job_queue_mutex must be held before calling this function
 */
PRIVATE job_t *get_job_from_job_queue(void)
{
    job_queue_t *job_queue = get_job_queue();
    job_t *job = NULL;

    if (job_queue->head) {
        job = job_queue->head;
        job_queue->head = job->next;
        job_queue->job_num--;
    }

    return job;
}

/*
 * return value: 0 : success
 *              -1 : fail
 */
API int add_job_to_job_queue(job_func_t job_func, void *arg)
{
    job_queue_t *job_queue = get_job_queue();
    job_t *job = malloc(sizeof(*job));
    if (job == NULL) {
        DEBUG("malloc error");
        return -1;
    }
    memset(job, 0, sizeof(*job));
    job->job_func = job_func;
    job->arg = arg;
    job->next = NULL;

    pthread_rwlock_rdlock(&(thread_pool.status_lock));
    if (thread_pool.status != THREAD_POOL_ACTIVE) {
        pthread_rwlock_unlock(&(thread_pool.status_lock));
        free(job);
        job = NULL;
        return -1;
    }
    pthread_rwlock_unlock(&(thread_pool.status_lock));

    pthread_mutex_lock(&(job_queue->job_queue_mutex));
    if (job_queue->head == NULL) {
        job_queue->head = job;
        job_queue->tail = job;
    } else {
        job_queue->tail->next = job;
        job_queue->tail = job;
    }
    job_queue->job_num++;
    pthread_mutex_unlock(&(job_queue->job_queue_mutex));

    pthread_cond_broadcast(&(job_queue->job_queue_cond));

    return 0;
}

/********************* thread queue methods ********************/

PRIVATE inline thread_queue_t *get_thread_queue(void)
{
    return &(thread_pool.thread_queue);
}

/* 
 * worker thread
 */
PRIVATE void *thread_work(void *arg)
{
    job_queue_t *job_queue = get_job_queue();
    thread_queue_t *thread_queue = get_thread_queue();
    thread_struct_t *thread_struct = (thread_struct_t *)arg;
    job_t *job = NULL;
    int oldstate = 0;

    while (1) {
        pthread_mutex_lock(&(job_queue->job_queue_mutex));
        while (!job_queue->job_num) {
            printf("thread: %x sleep\n", thread_struct->tid);
            pthread_cond_wait(&(job_queue->job_queue_cond), &(job_queue->job_queue_mutex));
            printf("thread: %x wake\n", thread_struct->tid);
        }
        /* get a job from job queue and execute */
        job = get_job_from_job_queue();
        pthread_mutex_unlock(&(job_queue->job_queue_mutex));

        /* unable to cancel while executing the job */
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
        if (job) {
            job->job_func(job->arg);
            free(job);
            job = NULL;
        }
        pthread_setcancelstate(oldstate, NULL);

        /* insert cancel point */
        pthread_testcancel();
    }

    return NULL;
}

/*
 * thread_queue->thread_queue_mutex must be held before calling this function
 * return value: 0 : success
 *              -1 : fail
 */
PRIVATE int __add_thread_to_thread_queue(void)
{
    int ret = 0;
    thread_queue_t *thread_queue = get_thread_queue();
    thread_struct_t *thread_struct = malloc(sizeof(*thread_struct));
    if (thread_struct == NULL) {        
        DEBUG("malloc error");
        return -1;
    }
    memset(thread_struct, 0, sizeof(*thread_struct));

    ret = pthread_create(&(thread_struct->tid), NULL, thread_work, (void *)thread_struct);
    if (ret != 0) {
        DEBUG("pthread_create error");
        return -1;
    }

    if (thread_queue->head == NULL) {
        thread_queue->head = thread_struct;
        thread_queue->tail = thread_struct;
    } else {
        thread_queue->tail->next = thread_struct;
        thread_queue->tail = thread_struct;
    }
    thread_queue->thread_num++;

    return 0;
}

/*
 * return value: 0 : success
 *              -1 : fail
 */
API int add_thread_to_thread_queue(int num)
{
    int i = 0, ret = 0;
    thread_queue_t *thread_queue = get_thread_queue();

    pthread_rwlock_rdlock(&(thread_pool.status_lock));
    if (thread_pool.status != THREAD_POOL_ACTIVE) {
        pthread_rwlock_unlock(&(thread_pool.status_lock));
        return -1;
    }
    pthread_rwlock_unlock(&(thread_pool.status_lock));

    for (i = 0; i < num; i++) {
        pthread_mutex_lock(&(thread_queue->thread_queue_mutex));
        ret = __add_thread_to_thread_queue();
        if (ret != 0) {
            pthread_mutex_unlock(&(thread_queue->thread_queue_mutex));
            DEBUG("add_thread_to_thread_queue error");
            return -1;
        }
        pthread_mutex_unlock(&(thread_queue->thread_queue_mutex));
    }

    return 0;
}

/*
 * return value: 0 : success
 *              -1 : fail
 */
PRIVATE int __del_thread_from_thread_queue(void)
{
    int ret = 0;
    thread_queue_t *thread_queue = get_thread_queue();
    thread_struct_t *thread_struct = NULL;

    if (thread_queue->head) {
        thread_struct = thread_queue->head;
        thread_queue->head = thread_struct->next;
        thread_queue->thread_num--;
        ret = pthread_cancel(thread_struct->tid);
        if (ret != 0) {
            DEBUG("pthread_cancel error\n");
            return -1;
        }
        ret = pthread_join(thread_struct->tid, NULL);
        if (ret != 0) {
            DEBUG("pthread_join error\n");
            return -1;
        }
        free(thread_struct);
        thread_struct = NULL;
    }

    return 0;
}

/*
 * @num: -1: delete all thread from thread queue;
 *       >0: delete number of @num threads from thread queue;
 * return value: 0 : success
 *              -1 : fail
 */
PRIVATE int del_thread_from_thread_queue(int num)
{
    thread_queue_t *thread_queue = get_thread_queue();
    int i = 0, ret = 0;

    pthread_mutex_lock(&(thread_queue->thread_queue_mutex));
    if ((num < 0) || (num > thread_queue->thread_num)) {
        num = thread_queue->thread_num;
    }

    for (i = 0; i < num; i++) {
        ret = __del_thread_from_thread_queue();
        if (ret < 0) {
            pthread_mutex_unlock(&(thread_queue->thread_queue_mutex));
            DEBUG("__del_thread_from_thread_queue error");
            return -1;
        }
    }
    pthread_mutex_unlock(&(thread_queue->thread_queue_mutex));
    return 0;
}

/*
 * return value: 0 : success
 *              -1 : fail
 */
PRIVATE int thread_queue_init(int num)
{
    int i = 0, ret = 0;
    thread_queue_t *thread_queue = get_thread_queue();
    pthread_mutex_init(&(thread_queue->thread_queue_mutex), NULL);

    ret = add_thread_to_thread_queue(num);
    if (ret != 0) {
        DEBUG("add_thread_to_thread_queue error");
        return -1;
    }

    return 0;
}

/*
 * return value: 0 : success
 *              -1 : fail
 */
PRIVATE int thread_queue_destroy(void)
{
    return del_thread_from_thread_queue(-1);
}

API int get_total_thread_num(void)
{
    thread_queue_t *thread_queue = get_thread_queue();
    int total = 0;

    pthread_mutex_lock(&(thread_queue->thread_queue_mutex));
    total = thread_queue->thread_num;
    pthread_mutex_unlock(&(thread_queue->thread_queue_mutex));

    return total;
}

/************************* thread pool methods **************************/

/*
 * return value: 0 : success
 *              -1 : fail
 */
API int thread_pool_init(int num)
{
    DEBUG("thread_pool_init");
    int ret = 0;

    thread_pool.status = THREAD_POOL_ACTIVE;
    job_queue_init();
    ret = thread_queue_init(num);
    if (ret != 0) {
        DEBUG("thread_queue_init error");
        return -1;
    }

    pthread_rwlock_init(&(thread_pool.status_lock), NULL);

    return ret;
}

/*
 * @wait: 0: destroy the job queue immediately ignore wether it's empty or not
 *        1: wait all the job on the job queue to complete
 * return value: 0 : success
 *              -1 : fail
 *
 */
API int thread_pool_destroy(int wait)
{
    thread_queue_t * thread_queue = get_thread_queue();
    int ret = 0;

    pthread_rwlock_wrlock(&(thread_pool.status_lock));
    thread_pool.status = THREAD_POOL_DESTROYING;
    pthread_rwlock_unlock(&(thread_pool.status_lock));

    job_queue_destroy(wait);

    ret = thread_queue_destroy();
    if (ret < 0) {
        DEBUG("thread_queue_destroy error");
        return -1;
    }

    return 0;
}
