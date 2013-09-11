/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  stk_setproctitle.h:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  set process title header.
 */

#ifndef __STK_SETPROCTITLE_H
#define __STK_SETPROCTITLE_H

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#define stk_init_setproctitle(argc, argv)
#define stk_setproctitle setproctitle
#elif defined(__sun)
#define stk_init_setproctitle(argc, argv)
#define stk_setproctitle
#else
#ifdef __linux__
int stk_init_setproctitle(int argc, char **argv);
void stk_setproctitle(char *title);
void stk_setproctitle2(char *prog, char *title);
#endif
#endif

#endif /*__STK_SETPROCTITLE_H*/
