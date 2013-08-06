/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_module.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for stk_module.
 */

#include <stdlib.h>

#include "stk_module.h"
#include "stk_log.h"

extern stk_module_t stk_test_module;
extern stk_module_t stk_test_module2;
extern stk_module_t stk_test_module3;

stk_module_t *stk_modules[] = {
    /* put your module here */
    &stk_test_module,
    &stk_test_module2,
    &stk_test_module3,
    NULL
};

int main(int argc, char *argv[])
{
    if (stk_init_modules() == STK_MODULE_ERROR) {
        stk_log_error("init modules failed!");
        stk_exit_modules();
        exit(1);
    }
    stk_log_info("other process here................");

    stk_exit_modules();

    return 0;
}
