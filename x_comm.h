/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  x_comm.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  x common.
 */

#ifndef __X_COMM_H
#define __X_COMM_H

#include <unistd.h>
#include <stdint.h>

typedef intptr_t        x_int_t;
typedef uintptr_t       x_uint_t;
typedef unsigned char   u_char;

#define  XC_OK          0     
#define  XC_ERROR      -1     
#define  XC_AGAIN      -2     
#define  XC_BUSY       -3     
#define  XC_DONE       -4     
#define  XC_DECLINED   -5     
#define  XC_ABORT      -6

#define x_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define x_align_ptr(p, a)         \
  (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#endif /*__X_COMM_H*/
