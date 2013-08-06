/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_signal.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: signal API.
 */

#ifndef __STK_SIGNAL_H
#define __STK_SIGNAL_H

#include <signal.h>

typedef struct {
    int     signo;                  /* signal number */
    char   *signame;                /* signal name */
    char   *name;                   /* process name */
    void  (*handler)(int signo);    /* process handler */
} stk_signal_t;

int stk_init_signals();

#endif /*__STK_SIGNAL_H*/
