/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  x_pool.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  x pool.
 */

#ifndef __X_POOL_H
#define __X_POOL_H

#include "x_comm.h"

#define x_pagesize (4 * 1024)
#define X_ALIGNMENT   sizeof(unsigned long)

#define X_MAX_ALLOC_FROM_POOL (x_pagesize - 1)
#define X_DEFAULT_POOL_SIZE   (16 * 1024)
#define X_MIN_PO0L_SIZE                                                       \
  (sizeof(x_pool_t) + 2 * sizeof(x_pool_large_t))

typedef void (*x_pool_cleanup_pt)(void *data);

typedef struct x_pool_s x_pool_t;
typedef struct x_pool_large_s x_pool_large_t;
typedef struct x_pool_cleanup_s x_pool_cleanup_t;

struct x_pool_large_s {
  x_pool_large_t    *next;
  void              *alloc;
};

struct x_pool_cleanup_s {
  x_pool_cleanup_pt   handler;
  void                *data;
  x_pool_cleanup_t    *next;
};

struct x_pool_s {
  u_char              *last;
  u_char              *end;
  x_pool_t            *current;
  x_pool_t            *next;
  x_pool_large_t      *large;
  x_pool_cleanup_t    *cleanup;
};


x_pool_t *x_create_pool(size_t size);
void x_destroy_pool(x_pool_t *pool);

void *x_palloc(x_pool_t *pool, size_t size);
void *x_pcalloc(x_pool_t *pool, size_t size);
x_int_t x_pfree(x_pool_t *pool, void *p);

x_pool_cleanup_t *x_pool_cleanup_add(x_pool_t *pool, size_t size);

x_pool_t *x_create_pool_recursive(x_pool_t *pool, size_t size);

void x_show_pool(x_pool_t *p);

#endif /*__X_POOL_H*/
