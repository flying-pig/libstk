/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_threads.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for stk_threads.
 */

#include <stdio.h>
#include <pthread.h>

#include "stk_threads.h"
#include "stk_log.h"
#include "stk_pool.h"

#define THREAD_STACK_SIZE   128 * 1024 

void *test_func(void *arg)
{
    char *msg = (char *)arg;

    stk_log_debug("msg:[%s]", msg);

    return NULL;
}
int main(int argc, char *argv[])
{
    int i;
    stk_init_threads(10, THREAD_STACK_SIZE);
    stk_thread_t *thread;
    stk_pool_t *pool;
    char *msg;
    pthread_t tid[10] = {0};

    pool = stk_create_pool();

    for (i = 0; i < 10; ++i) {
        msg = stk_pcalloc(pool, 128);
        snprintf(msg, 127, "test msg[%d] aaaaaaaaaaaaaaaaaa", i);
        thread = stk_thread_get();
        stk_create_thread(thread, test_func, msg);
        tid[i] = stk_thread_tid(thread);
    }
    
    for (i = 0; i < 10; ++i) {
        pthread_join(tid[i], NULL);
    }
    stk_destroy_pool(pool);
    stk_exit_threads();

    return 0;
}
