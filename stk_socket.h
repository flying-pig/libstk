/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_socket.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: socket.
 */

#ifndef __STK_SOCKET_H
#define __STK_SOCKET_H

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

#define STK_LISTEN_BACKLOG  1024

#define stk_nonblocking(s)  fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK)
#define stk_blocking(s)     fcntl(s, F_SETFL, fcntl(s, F_GETFL) & ~O_NONBLOCK)
#define stk_socket_connect_nonblocking(s, ip, port) \
    stk_socket_connect(s, ip, port, 1)
#define stk_socket_connect_blocking(s, ip, port) \
    stk_socket_connect(s, ip, port, 0)

typedef int stk_socket_t;


stk_socket_t stk_listening_init(const char *ip, int port);
int stk_socket_connect(stk_socket_t conn_fd, const char *ip,
                       int port, int flag);
stk_socket_t stk_accept(stk_socket_t listening_fd,
                        struct sockaddr_in *client_addr);
int stk_socket_is_nonblocking(stk_socket_t sockfd);
int stk_socket_keepalive(stk_socket_t sockfd);


#endif /*__STK_SOCKET_H*/
