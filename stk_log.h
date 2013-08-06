/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_log.h:  Chen Huaying chenhuaying@sina.cn
 *
 *  simple tool kit: log.
 */

#ifndef __STK_LOG_H
#define __STK_LOG_H

#include <stdarg.h>

#define DEBUG
#ifdef DEBUG
#define stk_log_error(fmt, args...) stk_log_msg(LIGHT | F_RED, fmt"\n", ##args)
#define stk_log_debug(fmt, args...) stk_log_msg(LIGHT | F_BLUE, fmt"\n", ##args)
#define stk_log_info(fmt, args...) stk_log_msg(LIGHT | F_PURPLE, fmt"\n", ##args)
#else
#define stk_log_error(fmt, args...) 
#define stk_log_debug(fmt, args...) 
#define stk_log_info(fmt, args...)
#endif

#define LIGHT   0x1
#define NOMAL   0x2
#define L_MASK  0xf

#define F_RED     0x10
#define F_BLUE    0x20
#define F_PURPLE  0x30
#define F_YELLOW  0x40
#define F_MASK    0xf0

#define B_RED     0x100
#define B_BLUE    0x200
#define B_PURPLE  0x300
#define B_YELLOW  0x400
#define B_MASK    0xf00

#ifdef __cplusplus
extern "C" {
#endif
void stk_log_msg(unsigned int flag, char *fmt, ...);
void stk_log_vmsg(unsigned int flag, char *fmt, va_list ap);
void stk_log_out(unsigned int flag, char *fmt, ...);
#ifdef __cplusplus
}
#endif


#endif /*__STK_LOG_H*/
