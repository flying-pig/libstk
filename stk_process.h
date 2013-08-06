/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_process.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: spawn processes.
 */

#ifndef __STK_PROCESS_H
#define __STK_PROCESS_H

typedef pid_t       stk_pid_t;

stk_pid_t   stk_pid;

#define STK_PROCESS_MASTER     1
#define STK_PROCESS_WORKER     2

#define STK_INVALID_PID  -1

#define STK_SHUTDOWN_SIGNAL      QUIT
#define STK_TERMINATE_SIGNAL     TERM

#define stk_signal_helper(n)     SIG##n
#define stk_signal_value(n)      stk_signal_helper(n)

typedef void (*stk_spawn_proc_pt)(void *data);

typedef struct {
    stk_pid_t           pid;
    int                 status;

    stk_spawn_proc_pt   proc;
    void               *data;
    char               *name;

    unsigned            respawn:1;
    unsigned            just_spawn:1;
    unsigned            detached:1;
    unsigned            exiting:1;
    unsigned            exited:1;
} stk_process_t;

#define STK_PROCESS_NORESPAWN     -1
#define STK_PROCESS_JUST_SPAWN    -2
#define STK_PROCESS_RESPAWN       -3
#define STK_PROCESS_JUST_RESPAWN  -4
#define STK_PROCESS_DETACHED      -5

#define STK_MAX_PROCESSES          1024


#define stk_getpid      getpid

stk_pid_t stk_spawn_process(stk_spawn_proc_pt proc,
        void *data, char *name, int respawn);
void stk_master_process();
void stk_master_signal_handler(int signo);
void stk_set_worker_process(stk_spawn_proc_pt proc, void *data, char *name);
void stk_set_worker_num(int num);
void stk_signal_worker_process(int signo);

#endif /*__STK_PROCESS_H*/
