/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_socket.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: socket.
 */

#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "stk_socket.h"
#include "stk_log.h"
#include "stk_comm.h"

stk_socket_t stk_listening_init(const char *ip, int port)
{
    int flag;
    stk_socket_t listening_fd;
    struct sockaddr_in listening_addr;

    bzero(&listening_addr, sizeof(struct sockaddr_in));
    listening_addr.sin_family = AF_INET;
    listening_addr.sin_addr.s_addr = ((ip == NULL) ? htonl(INADDR_ANY)
                                                   : inet_addr(ip));
    listening_addr.sin_port = htons(port);

    listening_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_fd < 0) {
        stk_log_error("create socket error! errno:[%d]", errno);
        return listening_fd;
    }

    flag = 1;
    if (setsockopt(listening_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))
        < 0)
    {
        stk_log_error("set listening_fd reuseaddr failed! errno:[%d]", errno);
        goto socket_failed;
    }
#if 0 /* if set listening socket non blocking */
    if (stk_nonblocking(listening_fd) < 0) {
        stk_log_error("set listening_fd non blocking failed!");
        goto socket_failed;
    }
#endif

    if (bind(listening_fd, (struct sockaddr *)&listening_addr,
             sizeof(listening_addr))
        < 0)
    {
        stk_log_error("bind listening socket failed! errno:[%d]", errno);
        goto socket_failed;
    }
    if (listen(listening_fd, STK_LISTEN_BACKLOG) < 0) {
        stk_log_error("listen listening socket failed! errno:[%d]", errno);
        goto socket_failed;
    }
    stk_log_info("start listening socket: [%d]", listening_fd);

    return listening_fd;

socket_failed:
    close(listening_fd);
    return STK_ERR;
}

/* flag: 0 blocking socket, 1 non blocking soecket */
int stk_socket_connect(stk_socket_t conn_fd, const char *ip,
                                int port, int flag)
{
    struct sockaddr_in remote_addr;
#if 0
    stk_socket_t conn_fd;

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_fd < 0) {
        stk_log_error("create socket failed! errno:[%d]", errno);
        return conn_fd;
    }
#endif

    if (flag) {
        if (stk_nonblocking(conn_fd)) {
            stk_log_error("set connect fd failed! errno:[%d]", errno);
            goto socket_failed;
        }
    }

    bzero(&remote_addr, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr(ip);
    remote_addr.sin_port = htons(port);

    if (connect(conn_fd, (struct sockaddr *)&remote_addr, sizeof(remote_addr))
        < 0)
    {
        stk_log_error("connect remote ip failed! errno:[%d]", errno);
        if (errno == EINPROGRESS) {
            return STK_DECLINED;
        }
        goto socket_failed;
    }

    return STK_OK;

socket_failed:
#if 0
    close(conn_fd);
#endif
    return STK_ERR;
}

stk_socket_t stk_accept(stk_socket_t listening_fd,
                        struct sockaddr_in *client_addr)
{
    /* TODO: for use STK_AGIN when accept errno is EAGAIN */
    stk_socket_t client_fd;
    socklen_t client_fd_len = sizeof(struct sockaddr_in);
    if ((client_fd = accept(listening_fd, (struct sockaddr *)client_addr,
                            &client_fd_len))
         < 0)
    {
        stk_log_error("listening_fd accept failed! errno:[%d]", errno);
        if (stk_socket_is_nonblocking(listening_fd)) {
             if (errno == EAGAIN) {
                 stk_log_debug("accept() not ready! errno:[%d]", errno);
                 return STK_AGAIN;
             }
        }
        return STK_ERR;
    }
    stk_log_info("accept a client[%d] connect!", client_fd);
    return client_fd;
}

int stk_socket_keepalive(stk_socket_t sockfd)
{
    /* JUST RETURN OK */
    return STK_OK;
}

int stk_socket_is_nonblocking(stk_socket_t sockfd)
{
    int flags;
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
        stk_log_error("get socket flags failed! errno:[%d]", errno);
        return STK_FALSE;
    }

    if (flags & O_NONBLOCK) 
        return STK_TRUE;
    return STK_FALSE;
}
