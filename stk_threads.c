/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_threads.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: threads pool.
 *
 *  note: just using for initializing threads
 */

#include <pthread.h>
#include <stdlib.h>

#include "stk_comm.h"
#include "stk_pool.h"
#include "stk_log.h"
#include "stk_threads.h"
#include "stk_async_queue.h"


struct stk_thread_s {
    pthread_t            tid;
    int                  initialized;
    void                *magic;
    stk_async_queue_t   *task_queue;
};

static pthread_attr_t       thr_attr;
static pthread_mutex_t      thr_mutex;
static stk_thread_t        *stk_threads;
static int                  nthreads;
static int                  max_threads;
static int                  free_index;

int stk_init_threads(int n, size_t size)
{
    int err;

    max_threads = n > STK_MAX_THREADS ? STK_MAX_THREADS : n;

    err = pthread_attr_init(&thr_attr);

    if (err != 0) {
        stk_log_error("pthread_attr_init() failed!");
        return STK_ERR;
    }

    err = pthread_mutex_init(&thr_mutex, NULL);
    
    if (err != 0) {
        stk_log_error("pthread_mutex_init() failed!");
        return STK_ERR;
    }

    err = pthread_attr_setstacksize(&thr_attr, size);

    if (err != 0) {
        stk_log_error("pthread_attr_setstacksize() failed!");
        return STK_ERR;
    }

    /* 
     * initialize global threads array, don't initialize the task_queue of
     * each thead, initialize it when you use it.
     */
    stk_threads = stk_alloc(sizeof(stk_thread_t) * STK_MAX_THREADS);
    if (stk_threads == NULL) {
        return STK_ERR;
    }


    return STK_OK;
}

/* 
 * this function doesn't ensure free the thread's queue element, it just free
 * the asynchronous queue it self, so there might be memory leak if there are 
 * many elements in the queue. But there is no matter of that, the exit() may
 * be called after this.
 */ 
void stk_exit_threads()
{
    int i;
    stk_thread_t *thread = NULL;

    for (i = 0; i < nthreads; ++i) {
        thread = &stk_threads[i];
        if (thread->initialized) {
            stk_async_queue_destroy(thread->task_queue);
        }
    }
    stk_free(stk_threads);
    stk_log_info("stk_exit_threads");
}


/* note: if stk_create_thread failed, just exit application */
int stk_create_thread(stk_thread_t *thread, 
        void *(*func)(void *arg), void *arg)
{
    int err;

    thread->magic = arg;
    if ((thread->task_queue = stk_async_queue_new()) == NULL) {
        stk_log_error("create a new task_queue for thread failed()!");
        return STK_ERR;
    }

    err = pthread_create(&thread->tid, &thr_attr, func, thread->magic);

    if (err != 0) {
        stk_async_queue_destroy(thread->task_queue);
        stk_log_error("pthread_create failed!");
        return STK_ERR;
    }
    thread->initialized = 1;

    ++nthreads;

    return err;
}

static int stk_threads_index()
{
    return free_index > nthreads ? -1 : free_index++;
}

stk_thread_t *stk_thread_get()
{
    int idx = stk_threads_index();
    if (idx < 0) {
        return NULL;
    }

    return &stk_threads[idx];
}

pthread_t stk_thread_tid(stk_thread_t *thread)
{
    return thread->tid;
}

int stk_thread_push_task(stk_thread_t *thread, void *data)
{
    int ret = stk_async_queue_push(thread->task_queue, data);

    return ret;
}

void *stk_thread_pop_task(stk_thread_t *thread)
{
    void *task;

    task = stk_async_queue_pop(thread->task_queue);

    return task;
}
