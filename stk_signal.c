/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_signal.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: signal API.
 */

#include <string.h>
#include <errno.h>

#include "stk_comm.h"
#include "stk_signal.h"
#include "stk_log.h"
#include "stk_pool.h"

extern stk_signal_t *signals;

/* the last element must be { 0, NULL, "", NULL } */

int stk_init_signals()
{
    stk_signal_t        *sig;
    struct sigaction     sa;

    for (sig = signals; sig->signo != 0; ++sig) {
        stk_memzero(&sa, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1) {
            stk_log_error("(%s) sigaction failed! %d %m", sig->signame, errno);
            return STK_ERR;
        }
    }
}

int stk_init_signal(int signo, void (*handler)(int signo))
{
    struct sigaction sa;
    stk_memzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(signo, &sa, NULL) == -1) {
        stk_log_error("(%d) sigaction failed! %d %m", signo, errno);
        return STK_ERR;
    }
}

int stk_signals_set(stk_signal_t *signals)
{
    stk_signal_t *sig;
    struct sigaction sa;

    for (sig = signals; sig->signo != 0; ++sig) {
        stk_memzero(&sa, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1) {
            stk_log_error("(%s) sigaction failed! %d %m", sig->signame, errno);
            return STK_ERR;
        }
    }
}
