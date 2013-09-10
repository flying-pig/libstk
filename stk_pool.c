/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_pool.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: memory pool.
 */

#include <stdlib.h>
#include <string.h>

#include "stk_list.h"
#include "stk_pool.h"
#include "stk_log.h"

#define STK_POOL_DEBUG 1

struct stk_pool_cleanup_s {
    stk_list_t              list;
    stk_pool_cleanup_pt     handler;
    void                   *data;
};

struct stk_pool_s {
    stk_list_t          cleanup;
};

void *stk_alloc(size_t size)
{                     
    void *p;

    p = malloc(size);
    if (p == NULL) {
        stk_log_error("malloc() %uz bytes failed", size);
    }

    return p;
}

void *stk_calloc(size_t size)
{
    void *p;

    p = stk_alloc(size);
    if (p) {
        stk_memzero(p, size);
    }

    return p;
}

stk_pool_t *stk_create_pool()
{
    stk_pool_t *pool;
    
    pool = stk_alloc(sizeof(stk_pool_t));

    if (pool) {
        stk_list_init(&pool->cleanup);
    }

    return pool;
}

stk_pool_t *stk_extend_pool(stk_pool_t *pool)
{
  stk_pool_t *p;
  p = stk_create_pool();
  stk_list_add(&p->cleanup, &pool->cleanup);
}

void stk_remove_pool(stk_pool_t *node)
{
  stk_list_del(&node->cleanup);
}

stk_pool_t *stk_create_pool_recursive(stk_pool_t *pool)
{
  if (pool == NULL) return stk_create_pool();

  stk_pool_t *p;
  p = stk_create_pool();
  if (p) stk_pool_cleanup_add(pool, (stk_pool_cleanup_pt)stk_destroy_pool, p);
  return p;
}

void stk_destroy_pool(stk_pool_t *pool)
{
    stk_reset_pool(pool);
    stk_free(pool);
}

void stk_reset_pool(stk_pool_t *pool)
{
    stk_pool_cleanup_t *clu_p;

    while (!stk_list_empty(&pool->cleanup)) {
        clu_p = stk_list_pop_entry(&pool->cleanup, stk_pool_cleanup_t, list);
        if (clu_p == NULL) {
            break;
        }
        if (clu_p) {
            (*(clu_p->handler))(clu_p->data);
        }
        stk_free(clu_p);
    }
}

int stk_pool_cleanup_add(stk_pool_t *pool,
                         stk_pool_cleanup_pt handler,
                         void *data)
{
    stk_pool_cleanup_t *cleanup = (stk_pool_cleanup_t *)
                                   stk_alloc(sizeof(stk_pool_cleanup_t));
    if (cleanup == NULL) {
        return -1;
    }
    cleanup->handler = handler;
    cleanup->data    = data;
    stk_list_add(&cleanup->list, &pool->cleanup);

    return 0;
}

int stk_pool_cleanup_del(stk_pool_t *pool, void *data)
{
    stk_pool_cleanup_t *itr = NULL;
    stk_list_for_each_entry(itr, &pool->cleanup, list) {
        if (itr->data == data) {
            /* free the data, is it needed? */
            (*(itr->handler))(data);
            stk_list_del(&itr->list);
            stk_free(itr);

            return 0;
        }
    }
    return 0;
}

void *stk_palloc(stk_pool_t *pool, size_t size)
{
    void *p;

    p = stk_alloc(size);
    if (p) {
        if (stk_pool_cleanup_add(pool, &stk_free, p) != 0) {
            stk_free(p);
            return NULL;
        }
    }

    return p;
}

void *stk_pcalloc(stk_pool_t *pool, size_t size)
{
    void *p;

    p = stk_palloc(pool, size);
    if (p) {
        stk_memzero(p, size);
    }
    
    return p;
}

#if STK_POOL_DEBUG
void stk_show_pool(stk_pool_t *pool)
{
    stk_list_t *ptr = NULL;
    stk_list_for_each(ptr, &pool->cleanup) {
        stk_log_debug("pool cleanup: next:[0X%X], prev[0X%X], cleanup:[0X%X]",
                      ptr->next, ptr->prev, ptr);
    }
}
#endif
