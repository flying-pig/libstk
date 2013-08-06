/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_module.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: module tool.
 */

#include <stdlib.h>
#include "stk_module.h"
#include "stk_log.h"

static int stk_max_module = 0;

extern stk_module_t *stk_modules[];

#define STK_MODULE_TEST
#undef  STK_MODULE_TEST

#ifdef  STK_MODULE_TEST
stk_module_t *stk_modules[] = {
    /* put your module here, and you should put this array in your own file */
    &stk_test_module,
    NULL
};
#endif

int stk_init_modules()
{
    int i = 0;
    stk_module_t *module = NULL;

    for (i = 0; stk_modules[i]; ++i) {
        module = stk_modules[i];
        if (module->init_module) {
            if (module->init_module() == STK_MODULE_ERROR) {
                stk_log_error("init module %s failed!", module->name);

                return STK_MODULE_ERROR;
            }
        }
        module->index = stk_max_module++;
    }

    stk_log_info("init modules successed!");
    stk_log_debug("stk_max_module = %d", stk_max_module);

    return STK_MODULE_OK;
}

void stk_exit_modules()
{
    int i = 0;
    stk_module_t *module = NULL;

    stk_log_debug("stk_max_module = %d", stk_max_module);

    for (i = 0; i < stk_max_module; ++i) {
        module = stk_modules[i];
        if (module->exit_module) {
            module->exit_module();
        }
    }

    stk_log_info("exit all inited modules!");
}
