/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  test_x_palloc.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  test for x palloc.
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "x_palloc.h"
#include "stk_log.h"

x_pool_t *g_pool;

int main(int argc, char *argv[])
{
  g_pool = x_create_pool(1024);
  stk_log_debug("g_pool addr: 0X%X", g_pool);

  char *g_name = NULL;
  char *g_attr = NULL;
  g_name = (char *)x_palloc(g_pool, 128 + 1);
  strncpy(g_name, "global pool", 128);
  x_show_pool(g_pool);
  puts("-----------------------\n");
  g_attr = (char *)x_pcalloc(g_pool, 128 + 1);
  strncpy(g_attr, "global attr none", 128);
  stk_log_debug("g_name: %s", g_name);
  stk_log_debug("g_attr: %s", g_attr);
  x_show_pool(g_pool);
  puts("-----------------------\n");

  char *block1 = (char *)x_pcalloc(g_pool, 916);
  snprintf(block1, 512, "(0x%p)aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", block1);
  x_show_pool(g_pool);
  puts("-----------------------\n");

  /* add a child  */
  x_pool_t *pool_a = x_create_pool(2048);
  stk_log_debug("pool_a addr: 0X%X", pool_a);
  x_pool_cleanup_t *c = x_pool_cleanup_add(g_pool, 0);
  assert(c == g_pool->cleanup);
  //c->handler = x_destroy_pool;
  g_pool->cleanup->handler = (x_pool_cleanup_pt)x_destroy_pool;
  g_pool->cleanup->data = pool_a;

  char *pool_a_name = x_pcalloc(pool_a, 256);
  strncpy(pool_a_name, "pool a, a child", 256);
  char *pool_a_attr = x_palloc(pool_a, 256);
  strncpy(pool_a_attr, "pool attr, a child", 256);

  x_pool_t *pool_b = x_create_pool_recursive(g_pool, 1024);
  stk_log_debug("pool_b addr: 0X%X", pool_b);
  char *pool_b_name = x_pcalloc(pool_b, 256);
  strncpy(pool_b_name, "pool b, a child", 256);
  char *pool_b_attr = x_palloc(pool_b, 256);
  strncpy(pool_b_attr, "pool attr, a child", 256);
  //stk_log_info("pool_a>>>>>>>>>>>>>>>>>>>>>");
  //x_show_pool(pool_a);
  //stk_log_info("pool_a end>>>>>>>>>>>>>>>>>>>>>");
  x_show_pool(g_pool);

  x_destroy_pool(g_pool);

  return 0;
}
