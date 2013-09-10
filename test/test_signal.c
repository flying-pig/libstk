/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_signal.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for signal.
 */

#include <unistd.h>
#include <assert.h>

#include "stk_log.h"
#include "stk_signal.h"

stk_signal_t *signals;

void test_signal_handler(int signo)
{
    assert(signo == SIGUSR1);

    stk_log_info("process SIGUSR1...............");
}

stk_signal_t signals_[] = {
    { SIGUSR1, "SIGUSR1", "", test_signal_handler},
    { SIGPIPE, "SIGPIPE, SIG_IGN", "", SIG_IGN },
    { 0, NULL, "", NULL }
};

int main(int argc, char *argv[])
{
    //signals = signals_;
    stk_signals_set(signals_);

    while (1) {
        sleep(10);
        stk_log_info("main loop ...");
    }

    return 0;
}
