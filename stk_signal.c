/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_signal.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: signal API.
 */

#include <string.h>

#include "stk_comm.h"
#include "stk_signal.h"
#include "stk_log.h"
#include "stk_pool.h"


/* the last element must be { 0, NULL, "", NULL } */
extern stk_signal_t signals[];

int stk_init_signals()
{
    stk_signal_t        *sig;
    struct sigaction     sa;

    for (sig = signals; sig->signo != 0; ++sig) {
        stk_memzero(&sa, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1) {
            stk_log_error("sigaction failed!");
            return STK_ERR;
        }
    }
}
