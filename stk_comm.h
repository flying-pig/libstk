/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  comm.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: common use.
 */

#ifndef __COMM_H
#define __COMM_H

#include <unistd.h>
#include <stdint.h>

typedef intptr_t        stk_int_t;
typedef uintptr_t       stk_uint_t;

#define STK_TRUE         1
#define STK_FALSE        0

#define STK_OK           0
#define STK_ERR         -1
#define STK_ERROR        STK_ERR
#define STK_DECLINED    -2
#define STK_AGAIN       -3

#define stk_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define stk_align_ptr(p, a)         \
  (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

int daemonize(int nochdir, int noclose);


#endif /*__COMM_H*/
