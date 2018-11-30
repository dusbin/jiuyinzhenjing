#include "thread_pool.h"
#include <assert.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
struct _thread_pool_task {
	thread_pool_callback_func cb;
	void* arg;
	struct list_head node;
};
static void* thread_pool_func(void* arg) {
	struct thread_pool* pool        = NULL;
	struct _thread_pool_task* task  = NULL;
	struct list_head* first_task;
	assert(arg);
	pool = (struct thread_pool*)arg;
	while (!pool->exit) {
		pthread_mutex_lock(&pool->task_cond_lock);
		//while (condition_is_false)
		printf("wait for signal...\n");
		pthread_cond_wait(&pool->task_cond, &pool->task_cond_lock);
		if (pool->exit) {
			pthread_mutex_unlock(&pool->task_cond_lock);
			break;
		}
		printf("exec for signal-------------------\n");
		pthread_mutex_unlock(&pool->task_cond_lock);
RETRY:
		/*从任务队列中取出一个任务*/
		pthread_mutex_lock(&pool->task_lock);
		if (list_empty(&pool->tasks)) {
			pthread_mutex_unlock(&pool->task_lock);
			continue;
		}
		first_task = pool->tasks.next;
		list_del(pool->tasks.next);
		pool->task_num--;
		pthread_mutex_unlock(&pool->task_lock);

		/*执行任务*/
		task = container_of(first_task, struct _thread_pool_task, node);
		pthread_mutex_lock(&pool->curr_running_task_num_lock);
		pool->curr_running_task_num++;
		pthread_mutex_unlock(&pool->curr_running_task_num_lock);
		(*(task->cb))(task->arg);
		pthread_mutex_lock(&pool->curr_running_task_num_lock);
		pool->curr_running_task_num--;
		pthread_mutex_unlock(&pool->curr_running_task_num_lock);
		if (task) {
			uxsino_free(task);
		}
		goto RETRY;
	}
	pthread_exit(NULL);
	return NULL;
}
struct thread_pool* thread_pool_create(int thread_num);
void thread_pool_release(struct thread_pool* pool);
int thread_pool_add_task(struct thread_pool* pool, thread_pool_callback_func cb, void* arg);
struct thread_pool* thread_pool_create(int thread_num) {
	struct thread_pool* pool = NULL;
	int i=0;
	assert(thread_num > 0);
	pool = uxsino_malloc(
			struct thread_pool, 
			sizeof(struct thread_pool),
			GFP_USER);
	if (!pool) goto ERROR;
	pthread_mutex_init(&pool->task_cond_lock, NULL);
	pthread_cond_init(&pool->task_cond, NULL);
	pthread_mutex_init(&pool->task_lock, NULL);
	pthread_mutex_init(&pool->curr_running_task_num_lock, NULL);
	pool->exit                          = FALSE;
	pool->curr_running_task_num         = 0;
	pool->thread_num                    = 0;
	pool->task_num                      = 0;
	INIT_LIST_HEAD(&pool->tasks);
	pool->threads = uxsino_malloc(
			pthread_t, 
			sizeof(pthread_t)*thread_num, 
			GFP_USER);
	if (!pool->threads) {
		goto ERROR;
	}
	pool->started = uxsino_malloc(int, thread_num*(sizeof(int)), GFP_USER);
        if (!pool->started) {
            goto ERROR;
        }
	for (i = 0; i < thread_num; i++) {
            pool->started[i] = FALSE;
		if (pthread_create(&pool->threads[i], NULL, thread_pool_func, (void *)pool)) {
			goto ERROR;
		}
		while (ESRCH == pthread_kill(pool->threads[i], 0)) {
			printf("~~~~~~~~~~~~thread not running...\n");
			usleep(10);
		}
		pool->thread_num++;
	}
	return pool;
ERROR:
	/*释放线程池内存*/
	thread_pool_release(pool);
	return NULL;
}
//thread_pool
void thread_pool_release(struct thread_pool* pool) {
	int i;
	struct list_head* pos, *n;
	struct _thread_pool_task* task;
	if (pool) {
		pool->exit = TRUE;  /*退出标志*/
		if (pool->threads) {
			/*通知所有线程退出*/
			pthread_mutex_lock(&pool->task_cond_lock);
			pthread_cond_broadcast(&pool->task_cond);
			pthread_mutex_unlock(&pool->task_cond_lock);
			/*等所有线程退出*/
			for (i = 0; i < pool->thread_num; i++) {
				pthread_join(pool->threads[i], NULL);
			}
			/*释放线程ID内存*/
			uxsino_free(pool->threads);
			pool->threads = NULL;
		}
		/*销毁资源*/
		pthread_mutex_destroy(&pool->task_cond_lock);
		pthread_cond_destroy(&pool->task_cond);
		pthread_mutex_destroy(&pool->task_lock);
		pthread_mutex_destroy(&pool->curr_running_task_num_lock);

		/*释放任务队列*/
		list_for_each_safe(pos, n, &pool->tasks){
			task = container_of(pos, struct _thread_pool_task, node);
			list_del(pos);
			uxsino_free(task);
		}
        if (pool->started) {
			uxsino_free(pool->started);
		}
		/*释放线程池内存*/
		uxsino_free(pool);
		pool = NULL;
	}
}
int thread_pool_add_task(struct thread_pool* pool, thread_pool_callback_func cb, void* arg) {
	struct _thread_pool_task* task;
	assert(pool && cb);
	task = uxsino_malloc(
			struct _thread_pool_task,
			sizeof(struct _thread_pool_task),
			GFP_USER);
	if (!task) return FUNC_ERR;
	task->cb        = cb;
	task->arg       = arg;
	pthread_mutex_lock(&pool->task_lock);
	list_add_tail(&task->node, &pool->tasks);
	pool->task_num++;
	pthread_mutex_unlock(&pool->task_lock);
	printf("add a task to thread pool...\n");
	/*通知所有线程有任务到达*/
	pthread_mutex_lock(&pool->task_cond_lock);
	pthread_cond_broadcast(&pool->task_cond);
	pthread_mutex_unlock(&pool->task_cond_lock);

	return FUNC_OK;
}


