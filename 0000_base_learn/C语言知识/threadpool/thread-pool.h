#ifndef THREAD_POOL_H
#define THREAD_POOL

#define API
#define PRIVATE static

typedef void (*job_func_t)(void *arg);

API int thread_pool_init(int num);
API int thread_pool_destroy(int wait);
API int add_job_to_job_queue(job_func_t job_func, void *arg);
API int add_thread_to_thread_queue(int num);
API int get_total_thread_num(void);

#endif
