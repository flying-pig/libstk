/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  x_pool.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  x pool.
 */

#include <stdio.h>
#include <assert.h>

#include "x_comm.h"
#include "x_palloc.h"
#include "x_alloc.h"


x_pool_t *x_create_pool(size_t size)
{
  x_pool_t *p;

  p = (x_pool_t *)x_alloc(size);
  if (p == NULL) {
    return NULL;
  }

  p->last = (u_char *) p + sizeof(x_pool_t);
  p->end = (u_char *) p + size;
  p->current = p;
  p->next = NULL;
  p->large = NULL;
  p->cleanup = NULL;

  return p;
}

void x_destroy_pool(x_pool_t *pool)
{
  x_pool_t          *p, *n;
  x_pool_large_t    *l;
  x_pool_cleanup_t  *c;

  for (c = pool->cleanup; c; c = c->next) {
    if (c->handler) {
      c->handler(c->data);
    }
  }

  for (l = pool->large; l; l = l->next) {
    if (l->alloc) {
      x_free(l->alloc);
    }
  }

  for (p = pool, n = pool->next; /* void */; p = n, n = n->next) {
    x_free(p);
    
    if (n == NULL) {
      break;
    }
  }
}

void *x_palloc(x_pool_t *pool, size_t size)
{
  u_char *m;
  x_pool_t *p, *n, *current;
  x_pool_large_t *large;
  if (size <= (size_t)X_MAX_ALLOC_FROM_POOL
      && size <= (size_t) (pool->end - (u_char *) pool)
                 - (size_t) x_align_ptr(sizeof(x_pool_t), X_ALIGNMENT))
  {
    p = pool->current;
    current = p;


    for ( ;; ) {
      m = x_align_ptr(p->last, X_ALIGNMENT);

      if ((size_t) (p->end - m) >= size) {
        p->last = m + size;
        return m;
      }

      if ((size_t) (p->end - m) < X_ALIGNMENT) {
        current = p->next;
      }

      if (p->next == NULL) {
        break;
      }

      p = p->next;
      pool->current = current;
    }

    n = x_create_pool((size_t) (p->end - (u_char *)p));
    if (n == NULL) {
      return NULL;
    }

    pool->current = current ? current : n;
    p->next = n;
    m = x_align_ptr(n->last, X_ALIGNMENT);
    n->last = m + size;

    return m;
  }

  p = x_alloc(size);
  if (p == NULL) {
    return NULL;
  }

  large = x_palloc(pool, sizeof(x_pool_large_t));
  if (large == NULL) {
    x_free(p);
    return NULL;
  }

  large->alloc = p;
  large->next = pool->large;
  pool->large = large;

  return p;
}

void *x_pcalloc(x_pool_t *pool, size_t size)
{
  void *p;

  p = x_palloc(pool, size);
  if (p) {
    x_memzero(p, size);
  }
  return p;
}

x_int_t x_pfree(x_pool_t *pool, void *p)
{
  x_pool_large_t *l;

  for (l = pool->large; l; l = l->next) {
    if (l = l->alloc) {
      x_free(l->alloc);
      l->alloc = NULL;
      return XC_OK;
    }
  }

  return XC_DECLINED;
}

x_pool_cleanup_t *x_pool_cleanup_add(x_pool_t *p, size_t size)
{
  x_pool_cleanup_t *c;

  c = x_palloc(p, sizeof(x_pool_cleanup_t));

  if (c == NULL) {
    return NULL;
  }

  if (size) {
    c->data = x_palloc(p, size);
    if (c->data == NULL) {
      return NULL;
    }

  } else {
    c->data = NULL;
  } 

  c->handler = NULL;
  c->next = p->cleanup;

  p->cleanup = c;

  return c;
}

x_pool_t *x_create_pool_recursive(x_pool_t *pool, size_t size)
{
  if (pool == NULL) return x_create_pool(size);

  x_pool_t *p;
  p = x_create_pool(size);
  if (p) {
    x_pool_cleanup_t *c = x_pool_cleanup_add(pool, 0);
    assert(c == pool->cleanup);
    pool->cleanup->handler = (x_pool_cleanup_pt)x_destroy_pool;
    pool->cleanup->data = p;
  }
  return p;
}

void x_show_pool(x_pool_t *p)
{
  x_pool_t *n;
  x_pool_large_t *l;
  x_pool_cleanup_t *c;

  for (n = p; n; n = n->next) {
    printf("pool info\n");
    printf("pool(0x%p) last(0x%p) end(0x%p)\n", n, n->last, n->end);
    printf("large info\n");
    for (l = n->large; l; l = l->next) {
      printf("alloc(0x%p) next(0x%p)\n", l->alloc, l->next);
    }
    printf("cleanup info\n");
    for (c = n->cleanup; c; c = c->next) {
      printf("data (0x%p) next(%p)\n", c->data, c->next);
    }
  }
}
