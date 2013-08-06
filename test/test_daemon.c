/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_daemon.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: test for daemonize.
 */

#include "stk_comm.h"
#include "stk_log.h"

int main(int argc, char *argv[])
{
    stk_log_info("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    daemonize(1, 0);
    stk_log_info("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
    return 0;
}
