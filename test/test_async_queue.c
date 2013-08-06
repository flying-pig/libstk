/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_async_queue.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for asynchronous queue.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "stk_async_queue.h"
#include "stk_pool.h"
#include "stk_log.h"

typedef struct my_data {
    int     a;
    char    b[128];
} MY_DATA;
#define push_thread_log(fmt, args...)    \
    stk_log_debug("push thread: "fmt, ##args)
void *test_push_thread(void *arg)
{
    pthread_detach(pthread_self());
    stk_async_queue_t *queue = (stk_async_queue_t *)arg;
    MY_DATA *data = NULL;
    int i;
    for (i = 0; i < 4; ++i) {
        sleep(3);
        data = (MY_DATA *)stk_alloc(sizeof(MY_DATA));
        data->a = i * 1000;
        snprintf(data->b, 127, "test data%d .....", i);
        stk_async_queue_push(queue, data);
    }

    return (void *)1;
}

int main(int argc, char *argv[])
{
    stk_async_queue_t *queue = stk_async_queue_new();
    if (queue == NULL) {
        stk_log_error("create asynchronous queue failed!");
        return -1;
    }

#if 0
#endif
    pthread_t   tid;

    pthread_create(&tid, NULL, test_push_thread, queue);

    MY_DATA *itr;
#define STK_ASYNC_QUEUE_TRY_POP
#ifndef STK_ASYNC_QUEUE_TRY_POP
    do {
        itr = (MY_DATA *)stk_async_queue_pop(queue);
        if (itr == NULL) {
            stk_log_error("asynchronous pop return NULL.. error!");
            break;
        }
        stk_log_debug("data: a = %d, b = [%s]", itr->a, itr->b);
        stk_free(itr);
    } while(1);
#else
    do {
        itr = (MY_DATA *)stk_async_queue_try_pop(queue);
        if (itr == NULL) {
            stk_log_info("there is no data in the queue!");
        } else {
            stk_log_debug("data: a = %d, b = [%s]", itr->a, itr->b);
            stk_free(itr);
        }
        sleep(1);
    } while(1);
#endif

    stk_async_queue_destroy(queue);

    return 0;
}
