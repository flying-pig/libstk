/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_process.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  test for process.
 */

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <string.h>

#include "stk_comm.h"
#include "stk_log.h"
#include "stk_pool.h"
#include "stk_process.h"
#include "stk_signal.h"

stk_signal_t *signals;
extern int stk_terminate;

void test_signal_handler(int signo);

void test_worker_proc(void *data)
{
    //sigset_t    set;

    //sigemptyset(&set);
    //sigaddset(&set, SIGTERM);
    //sigprocmask(SIG_UNBLOCK, &set, NULL);
    for ( ;; ) {
        stk_log_debug("worker: do process here!");
        stk_log_info("%d stk_terminate = %d", getpid(), stk_terminate);
        sleep(3);
    }
}

stk_signal_t signals_[] = {
    { SIGALRM, "SIGALRM", "", stk_master_signal_handler},
    { SIGCHLD, "SIGCHLD", "", stk_master_signal_handler},
    { SIGUSR1, "SIGUSR1", "", test_signal_handler},
    { SIGTERM, "SIGTERM", "", stk_master_signal_handler},
    { SIGINT,  "SIGINT",  "", stk_master_signal_handler},
    { 0, NULL, "", NULL }
};

void test_signal_handler(int signo)
{
    stk_signal_t *sig;

    for (sig = signals_; sig->signo != 0; ++sig) {
        if (sig->signo == signo) {
            break;
        }
    }

    stk_log_info("receive signal %d (%s)", sig->signo, sig->signame); 

    switch (signo) {

        case SIGTERM:
            exit(2);
            break;
            
        case SIGUSR1:
            stk_log_info("signal usr1 process.......");
            break;
    }
}

int main(int argc, char *argv[])
{
    signals = signals_;
    stk_init_signals();
    stk_set_worker_num(2);
    stk_set_worker_process(test_worker_proc, NULL, "test_worker");
    stk_master_process();
    return 0;
}
