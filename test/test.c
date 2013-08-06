/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  test for libstk.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>

#include "stk_log.h"
#include "stk_socket.h"
#include "stk_comm.h"


int main(int argc, char *argv[])
{
    int port = 6666;
    char buf[1024] = {0};
    char buf2[1024] = {0};
    int nbytes = 0;
    struct sockaddr_in client_addr;

    stk_socket_t serv_fd = stk_listening_init(NULL, port);
    if (serv_fd < 0) {
        stk_log_error("stk_listening_init failed!");
        return STK_ERR;
    }
    stk_socket_t cli_fd = stk_accept(serv_fd, &client_addr);
    nbytes = read(cli_fd, buf, 1024);
    stk_log_debug("read: [%s] | nbytes = %d", buf, nbytes);
    snprintf(buf2, nbytes, "Huaying stk: [%s]", buf);
    write(cli_fd, buf, 1024);
    close(cli_fd);
    close(serv_fd);

    return 0;
}

