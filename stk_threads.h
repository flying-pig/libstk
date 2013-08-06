/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_threads.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: threads pool.
 */

#ifndef __STK_THREADS_H
#define __STK_THREADS_H

#include <sys/types.h>

#define STK_MAX_THREADS     128

typedef struct stk_thread_s stk_thread_t;

int stk_create_thread(stk_thread_t *thread,
        void *(*func)(void *arg), void *arg);
int stk_init_threads(int n, size_t size);
stk_thread_t *stk_thread_get();
void stk_exit_threads();
pthread_t stk_thread_tid(stk_thread_t *thread);
int stk_thread_push_task(stk_thread_t *thread, void *task);
void *stk_thread_pop_task(stk_thread_t *thread);

#endif /*__STK_THREADS_H*/
