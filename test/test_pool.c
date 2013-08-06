/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_pool.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for stk_pool.
 */

#include <string.h>

#include "stk_pool.h"
#include "stk_log.h"

stk_pool_t *g_pool;
extern void stk_show_pool(stk_pool_t *pool);

int main(int argc, char *argv[])
{
    g_pool = stk_create_pool();
    stk_log_debug("g_pool addr: 0X%X", g_pool);

    char *g_name = NULL;
    char *g_attr = NULL;
    g_name = (char *)stk_palloc(g_pool, 128 + 1);
    strncpy(g_name, "global pool", 128);
    g_attr = (char *)stk_pcalloc(g_pool, 128 + 1);
    strncpy(g_attr, "global attr none", 128);
    stk_log_debug("g_name: %s", g_name);
    stk_log_debug("g_attr: %s", g_attr);

    /* a child pool add to global pool */
    stk_pool_t *pool_a = stk_create_pool_r(g_pool);
    stk_log_debug("pool_a addr: 0X%X", pool_a);
    //stk_pool_cleanup_add(g_pool, (stk_pool_cleanup_pt)stk_destroy_pool, pool_a);

    char *pool_a_name = stk_pcalloc(pool_a, 256);
    strncpy(pool_a_name, "pool a, a child", 256);
    char *pool_a_attr = stk_palloc(pool_a, 256);
    strncpy(pool_a_attr, "pool attr, a child", 256);
    stk_log_info("pool_a>>>>>>>>>>>>>>>>>>>>>");
    stk_show_pool(pool_a);
    stk_log_info("pool_a end>>>>>>>>>>>>>>>>>>>>>");
    stk_show_pool(g_pool);

    stk_destroy_pool(g_pool);


    return 0;
}
