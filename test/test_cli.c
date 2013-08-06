/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  test_cli.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  test socket client.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "stk_log.h"
#include "stk_socket.h"
#include "stk_comm.h"

int main(int argc, char *argv[])
{
    int port = 3899;
    char buf[1024] = {0};

    stk_socket_t cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (stk_socket_connect_blocking(cli_fd, "127.0.0.1", port) < 0) {
        close(cli_fd);
        return -1;
    }
    stk_log_debug("please input what ever things!");
    fgets(buf, sizeof(buf), stdin);
    int n = write(cli_fd, buf, sizeof(buf));
    stk_log_debug("write [%d] nbytes", n);
    bzero(buf, sizeof(buf));
    n = read(cli_fd, buf, sizeof(buf));
    stk_log_debug("cli_fd read:[%s],len = %d", buf, n);

    close(cli_fd);
    return 0;
}
