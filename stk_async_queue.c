/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_async_queue.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: asynchronous queue.
 */

#include <stdlib.h>

#include "stk_log.h"
#include "stk_list.h"
#include "stk_comm.h"
#include "stk_pool.h"
#include "stk_async_queue.h"


typedef struct __queue_data_s __queue_data_t;

struct stk_async_queue_s {
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;
    stk_list_t          list;
    int                 length;
    stk_pool_t         *pool;
};

struct __queue_data_s {
    stk_list_t      list;
    void           *data;
};

static int __stk_async_queue_init(stk_async_queue_t *queue, stk_pool_t *pool)
{
    if (pthread_cond_init(&queue->cond, NULL)) {
        return STK_ERR;
    }
    if (pthread_mutex_init(&queue->mutex, NULL)) {
        return STK_ERR;
    }

    stk_list_init(&queue->list);
    queue->length = 0;
    queue->pool = pool;

    return STK_OK;
}

stk_async_queue_t *stk_async_queue_new()
{
    stk_async_queue_t   *queue;
    stk_pool_t          *pool;

    if ((pool = stk_create_pool()) == NULL) {
        return NULL;
    }

    queue = (stk_async_queue_t *)stk_palloc(pool, sizeof(stk_async_queue_t));
    if (queue) {
        if (__stk_async_queue_init(queue, pool) == STK_ERR) {
            stk_destroy_pool(pool);
            stk_free(queue);
        }
    }

    return queue;
}

int stk_async_queue_push(stk_async_queue_t *queue, void *data)
{
    int ret;
    __queue_data_t *__queue_data;

    __queue_data = (__queue_data_t *)stk_palloc(queue->pool,
                                                sizeof(__queue_data_t));
    if (__queue_data == NULL) {
        return STK_ERR;
    }
    __queue_data->data = data;

    pthread_mutex_lock(&queue->mutex);
    stk_list_add_tail(&__queue_data->list, &queue->list);
    pthread_cond_broadcast(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);

    return STK_OK;
}

void *stk_async_queue_pop(stk_async_queue_t *queue)
{
    void *data;
    __queue_data_t *elt;

    pthread_mutex_lock(&queue->mutex);
    if (stk_list_empty(&queue->list)) {
#ifdef STK_DEBUG
        stk_log_debug("if asynchronous queue has no element, return NULL");
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
#else
        stk_log_debug("the asynchronous queue has no element, wait for data");
        pthread_cond_wait(&queue->cond, &queue->mutex);
#endif
    }
    elt = stk_list_pop_entry(&queue->list, __queue_data_t, list);
    data = elt->data;
    stk_pool_cleanup_del(queue->pool, elt);
    pthread_mutex_unlock(&queue->mutex);

    return data;
} 

void *stk_async_queue_try_pop(stk_async_queue_t *queue)
{
    void *data;
    __queue_data_t *elt;

    pthread_mutex_lock(&queue->mutex);
    if (stk_list_empty(&queue->list)) {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }
    elt = stk_list_pop_entry(&queue->list, __queue_data_t, list);
    data = elt->data;
    stk_pool_cleanup_del(queue->pool, elt);
    pthread_mutex_unlock(&queue->mutex);

    return data;
}

void stk_async_queue_destroy(stk_async_queue_t *queue)
{
    stk_destroy_pool(queue->pool);
}
