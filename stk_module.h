/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_module.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: module.
 */

#ifndef __STK_MODULE_H
#define __STK_MODULE_H

#define STK_MODULE_V1        0

#define STK_MODULE_OK        0
#define STK_MODULE_ERROR    -1

typedef int (*stk_init_module_t)(void);
typedef void (*stk_exit_module_t)(void);

struct stk_module_s {
    int                     index;
    char                   *name;
    stk_init_module_t       init_module;
    stk_exit_module_t       exit_module;
};

typedef struct stk_module_s stk_module_t;
int stk_init_modules();
void stk_exit_modules();


#endif /*__STK_MODULE_H*/
