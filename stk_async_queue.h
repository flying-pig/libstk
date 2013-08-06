/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_async_queue.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  stk tool kit: asynchronous queue.
 */

#ifndef __STK_ASYNC_QUEUE_H
#define __STK_ASYNC_QUEUE_H

#include <pthread.h>

typedef struct stk_async_queue_s stk_async_queue_t;

stk_async_queue_t *stk_async_queue_new(void);
/* note: the data must be alloc with malloc, otherwise destroy may incorrect ? */
int stk_async_queue_push(stk_async_queue_t *queue, void *data);
void *stk_async_queue_pop(stk_async_queue_t *queue);
void *stk_async_queue_try_pop(stk_async_queue_t *queue);
/*
 * Do not delete the data which you push to the queue.
 * when you call the interface make sure the queue is empty,
 * and you should delete the data with yourown,
 * so the interface is unusefull. o(∩∩)o...
 */
void stk_async_queue_destroy(stk_async_queue_t *queue);


#endif /*__STK_ASYNC_QUEUE_H*/
