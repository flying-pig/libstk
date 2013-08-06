/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_pool.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: memory pool.
 */

#ifndef __STK_POOL_H
#define __STK_POOL_H

#include <sys/types.h>

typedef struct stk_pool_s stk_pool_t;
typedef struct stk_pool_cleanup_s stk_pool_cleanup_t;

typedef void (*stk_pool_cleanup_pt)(void *data);

stk_pool_t *stk_create_pool();
stk_pool_t *stk_extend_pool(stk_pool_t *pool);
stk_pool_t *stk_create_pool_recursive(stk_pool_t *pool);
void stk_destroy_pool(stk_pool_t *pool);
void stk_reset_pool(stk_pool_t *pool);

void *stk_palloc(stk_pool_t *pool, size_t size);
void *stk_pcalloc(stk_pool_t *pool, size_t size);
int stk_pool_cleanup_add(stk_pool_t *pool,
                         stk_pool_cleanup_pt handler, void *data);
int stk_pool_cleanup_del(stk_pool_t *pool, void *data);

void *stk_alloc(size_t size);
void *stk_calloc(size_t size);

#define stk_create_pool_r stk_create_pool_recursive

#define stk_free    free
#define stk_memzero(buf, n)       (void) memset(buf, 0, n)
#define stk_pfree               stk_pool_clean_del



#endif /*__STK_POOL_H*/
