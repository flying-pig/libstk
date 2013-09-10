/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_process.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: spawn processes.
 */

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "stk_comm.h"
#include "stk_log.h"
#include "stk_signal.h"
#include "stk_process.h"


static u_char   master_process[] = "master process";

sig_atomic_t  stk_reap;
sig_atomic_t  stk_sigalarm;
sig_atomic_t  stk_terminate;
#if 0
sig_atomic_t  stk_sigio;
sig_atomic_t  stk_quit;
sig_atomic_t  stk_debug_quit;
stk_uint_t    stk_exiting;
sig_atomic_t  stk_reconfigure;
sig_atomic_t  stk_reopen;
#endif

stk_process_t   stk_processes[STK_MAX_PROCESSES];
stk_process_t   worker_process;
int             stk_last_process;
int             stk_process_slot;
int             worker_processes_num;
int             stk_process;

extern stk_signal_t *signals;


static stk_uint_t stk_reap_children();
static void stk_start_worker_process(int n, stk_int_t type);
static void stk_process_get_status(void);

stk_pid_t stk_spawn_process(stk_spawn_proc_pt proc, void *data,
        char *name, int respawn)
{
    assert(proc != NULL);
    stk_pid_t   pid;
    int         s;
    sigset_t set;

    if (respawn >= 0) {
        s = respawn;
    } else {
        for (s = 0; s < stk_last_process; ++s) {
            if (stk_processes[s].pid == -1) {
                break;
            }
        }

        if (s == STK_MAX_PROCESSES) {
            stk_log_error("no more than %d processes can be spawned",
                          STK_MAX_PROCESSES);
            return STK_INVALID_PID;
        }
    }

    /* XXX TODO: master and worker process interact */

    stk_process_slot = s;

    pid = fork();

    switch (pid) {

        case -1:
            stk_log_error("fork failed while spawn \"%s\"", name);
            return STK_INVALID_PID;

        case 0:
            /* child process */
            stk_pid = stk_getpid();
            stk_process = STK_PROCESS_WORKER;
            sigemptyset(&set);
            if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
              stk_log_error("sigprocmask() failed, %d %m", errno);
            }
            proc(data);
            break;

        default:
            /* parent process */
            break;
    }

    stk_log_info("start %s %d", name, pid);
    stk_processes[s].pid = pid;
    stk_processes[s].exited = 0;
    
    if (respawn >= 0) {
        return pid;
    }

    stk_processes[s].proc = proc;
    stk_processes[s].data = data;
    stk_processes[s].name = name;
    stk_processes[s].exiting = 0;

    switch (respawn) {

        case STK_PROCESS_NORESPAWN:
            stk_processes[s].respawn = 0;
            stk_processes[s].just_spawn = 0;
            stk_processes[s].detached = 0;
            break;

        case STK_PROCESS_JUST_SPAWN:
            stk_processes[s].respawn = 0;
            stk_processes[s].just_spawn = 1;
            stk_processes[s].detached = 0;
            break;

        case STK_PROCESS_RESPAWN:
            stk_processes[s].respawn = 1;
            stk_processes[s].just_spawn = 0;
            stk_processes[s].detached = 0;
            break;

        case STK_PROCESS_JUST_RESPAWN:
            stk_processes[s].respawn = 1;
            stk_processes[s].just_spawn = 1;
            stk_processes[s].detached = 0;
            break;

        case STK_PROCESS_DETACHED:
            stk_processes[s].respawn = 0;
            stk_processes[s].just_spawn = 0;
            stk_processes[s].detached = 1;
            break;
    }

    if (s == stk_last_process) {
        stk_last_process++;
    }

    return pid;
}

void stk_master_process()
{
    sigset_t            set;
    struct itimerval    itv;
    stk_uint_t          delay;
    stk_uint_t          live;

    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGINT);
    sigaddset(&set, stk_signal_value(STK_SHUTDOWN_SIGNAL));
    sigaddset(&set, stk_signal_value(STK_TERMINATE_SIGNAL));

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        stk_log_error("sigprocmask() failed!");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&set);

    stk_start_worker_process(worker_processes_num, STK_PROCESS_RESPAWN);

    delay = 0;
    live = 1;
    stk_process = STK_PROCESS_MASTER;

    for ( ;; ) {
        /* XXX TODO: for add SIGINT process, now the delay has no use yet */
        if (delay) {
            if (stk_sigalarm) {
                delay *= 2;
                stk_sigalarm = 0;
            }

            stk_log_info("termination delay: %d", delay);
        }

        itv.it_interval.tv_sec = 0;
        itv.it_interval.tv_usec = 0;
        itv.it_value.tv_sec = delay / 1000;
        itv.it_value.tv_usec = (delay % 1000 ) * 1000;

        if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
            stk_log_error("setitimer() failed");
        }

        stk_log_info("sigsuspend");

        sigsuspend(&set);

        stk_log_info("wake up");

        if (stk_reap && !stk_terminate) {
            stk_reap = 0;
            sleep(3);
            stk_log_info("reap children");

            live = stk_reap_children();
        }

        if (!live && stk_terminate /*|| stk_quit) */) {
            stk_log_info("master process exit");
            exit(EXIT_SUCCESS);
        }

        if (stk_terminate) {
            if (delay == 0) {
                delay = 50;
            }

            if (delay > 1000) {
                stk_signal_worker_process(SIGKILL);
                live = 0;
            } else {
                stk_signal_worker_process(
                        stk_signal_value(STK_TERMINATE_SIGNAL));
            }


            continue;
        }

#if 0
        if (stk_quit) {
            stk_signal_worker_process(stk_signal_value(STK_SHUTDOWN_SIGNAL));

            continue;
        }
#endif

#ifdef  STK_PROCESS_RESTART_ENABLE
        if (stk_restart) {
            stk_start_worker_process(worker_processes_num, STK_PROCESS_RESPAWN);
            live = 1;
        }
#endif
    }
}

static stk_uint_t stk_reap_children()
{
    stk_int_t       i;
    stk_uint_t      live;

    live = 0;

    for (i = 0; i < stk_last_process; ++i) {
        stk_log_info("child: %d %d exiting: %d t: exited: %d detached: %d"
                     "respawn: %d just_spawn: %d",
                     i,
                     stk_processes[i].pid,
                     stk_processes[i].exiting,
                     stk_processes[i].exited,
                     stk_processes[i].detached,
                     stk_processes[i].respawn,
                     stk_processes[i].just_spawn);
        if (stk_processes[i].pid == -1) {
            continue;
        }

        if (stk_processes[i].exited) {
            
            if (stk_processes[i].respawn
                    && !stk_processes[i].exiting)
            {
                if (stk_spawn_process(stk_processes[i].proc,
                            stk_processes[i].data,
                            stk_processes[i].name, i)
                        == STK_INVALID_PID)
                {
                    stk_log_error("can't respawn %s", stk_processes[i].name);
                    continue;
                }

                live = 1;

                continue;
            }

        } else if (/*stk_processes[i].exiting ||*/!stk_processes[i].detached) {
            live = 1;
        }
    }

    return live;
}

static void stk_process_get_status(void)
{
    int             status;
    char           *process;
    stk_pid_t       pid;
    int             i;
    stk_uint_t      one;

    one = 0;

    for (;;) {
        pid = waitpid(-1, &status, WNOHANG);

        if (pid == 0) {
            return;
        }

        if (pid == -1) {
            if (errno == EINTR) {
                continue;
            }

            if (errno == ECHILD && one) {
                return;
            }

            stk_log_error("waitpid() failed! errno(%d)", errno);

            return;
        }

        one = 1;
        process = "unknown process";

        for (i = 0; i < stk_last_process; ++i) {
            if (stk_processes[i].pid == pid) {
                stk_processes[i].exited = 1;        /* one child exited */
                stk_processes[i].status = status;
                process = stk_processes[i].name;
                break;
            }
        }

        if (WTERMSIG(status)) {
#ifdef  WCOREDUMP
            stk_log_error("%s %d exited on signal %d%s",
                          process, pid, WTERMSIG(status),
                          WCOREDUMP(status) ? " (core dumped)" : "");
#else
            stk_log_error("%s %d exited on signal %d",
                          process, pid, WTERMSIG(status));
#endif
        } else {
            stk_log_error("%s %d exited with code %d",
                          process, pid, WEXITSTATUS(status));
        }
    }
}

void stk_master_signal_handler(int signo)
{
    stk_signal_t *sig;

    for (sig = signals; sig->signo != 0; ++sig) {
        if (sig->signo == signo) {
            break;
        }
    }

    switch (stk_process) {
      case STK_PROCESS_MASTER:
        switch (signo) {

          case SIGALRM:
            stk_sigalarm = 1;
            break;
          case SIGCHLD:
            stk_reap = 1;
            break;
          case SIGTERM:
          case SIGINT:
            stk_terminate = 1;
            break;
        }
        break;

      case STK_PROCESS_WORKER:
        switch (signo) {
          case SIGTERM:
          case SIGINT:
            stk_terminate = 1;
            break;
        }
    }

    stk_log_info("[%s] signal %d (%s) received!",
                 stk_process == STK_PROCESS_WORKER ? "worker" : "master",
                 signo, sig->signame);
    
    if (signo == SIGCHLD) {
        stk_process_get_status();
    }
}

void stk_signal_worker_process(int signo)
{
  int i;
  for (i = 0; i < stk_last_process; ++i) {
    if (kill(stk_processes[i].pid, signo) < 0) {
      int err = errno;
      stk_log_error("kill(%d %d), failed!", stk_processes[i].pid, signo);
      if (err == ESRCH) {
        stk_processes[i].exited = 1;
        stk_processes[i].exiting = 0;
        stk_reap = 1;
      }
    }
  }
}

static void stk_start_worker_process(int n, stk_int_t type)
{
    stk_int_t   i;

    stk_log_info("start worker processes!");

    for (i = 0; i < n; ++i) {
        stk_spawn_process(worker_process.proc, worker_process.data, 
                          worker_process.name, STK_PROCESS_RESPAWN);
        stk_log_info("current slot: %d", stk_process_slot);
    }
}

void stk_set_worker_process(stk_spawn_proc_pt proc, void *data, char *name)
{
    worker_process.proc = proc;
    worker_process.data = data;
    worker_process.name = name;
}

void stk_set_worker_num(int num)
{
    worker_processes_num = num;
}
