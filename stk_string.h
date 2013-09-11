/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  stk_string.h:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  simple tool kit: string utils.
 */

#ifndef __STK_STRING_H
#define __STK_STRING_H

char *strtrim_right(char *p, char c);
char *strtrim_left(char *p, char c);
char *strtrim(char *p, char c);

char *trim_right_m(char *p);
char *trim_left_m(char *p);
char *trim_m(char *p);

char *trim_right_r(char *p);
char *trim_left_r(char *p);
char *trim_r(char *p);

size_t trim_right(char *out, size_t len, const char *p);
size_t trim_left(char *out, size_t len, const char *p);
size_t trim(char *out, size_t len, const char *p);

/* copy at most n bytes include '\0', ensure null-terminate */
u_char *stk_cpystrn(u_char *dst, u_char *src, size_t n);

#endif /*__STK_STRING_H*/
