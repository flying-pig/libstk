/*
 *  Copyright (c) 2010 SINA Corporation, All Rights Reserved.
 *
 *  stk_log.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  simple tool kit: log.
 */

#include <stdio.h>
#include <stdarg.h>

#include "stk_log.h"

#define COLOR_NULL {0, ""}

typedef struct component_s {
    char *fmt;
    void (*out_put)(unsigned int flag, char *fmt,...);
    int (*component)(unsigned int flag);
} component_t;

typedef struct log_color_s {
    unsigned int flag;
    char color_mode[8];
} log_color_t;

static log_color_t mode_color[] = {
    {LIGHT, "1"},
    {NOMAL, "0"},

    COLOR_NULL
};

static log_color_t front_color[] = {
    {F_RED,     "31"},
    {F_BLUE,    "34"},
    {F_PURPLE,  "35"},
    {F_YELLOW,  "33"},

    COLOR_NULL
};

static log_color_t bground_color[] = {
    {B_RED,     "41"},
    {B_BLUE,    "44"},
    {B_PURPLE,  "45"},
    {B_YELLOW,  "43"},

    COLOR_NULL
};

void stk_log_out(unsigned int flag, char *fmt,...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void stk_log_msg(unsigned int flag, char *fmt,...)
{
    unsigned int mode_f = flag & L_MASK;
    unsigned int color_f = flag & F_MASK;
    unsigned int bground_f = flag & B_MASK;

    char *mode = NULL;
    char *color = NULL;
    char *back_ground = NULL;

    int i = 0;
    while (mode_color[i].flag != 0) {
        if (mode_f == mode_color[i].flag) {
            mode = mode_color[i].color_mode;
            break;
        }
        ++i;
    }

    if (mode_color[i].flag ==0) {
        mode = "0";
    }
    i = 0;

    while (front_color[i].flag != 0) {
        if (color_f == front_color[i].flag) {
            color = front_color[i].color_mode;
            break;
        }
        ++i;
    }
    if (front_color[i].flag == 0) {
        color = "37";
    }
    i = 0;

    while (bground_color[i].flag != 0) {
        if (bground_f == bground_color[i].flag) {
            back_ground = bground_color[i].color_mode;
            break;
        }
        ++i;
    }
    if (bground_color[i].flag == 0) {
        back_ground = "40";
    }

    char fmt_buf[1024] = {0};
    sprintf(fmt_buf, "\033[%s;%s;%sm%s\033[0m", mode, color,
            back_ground, fmt);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt_buf, args);
    va_end(args);
}

void stk_log_vmsg(unsigned int flag, char *fmt, va_list ap)
{
    unsigned int mode_f = flag & L_MASK;
    unsigned int color_f = flag & F_MASK;
    unsigned int bground_f = flag & B_MASK;

    char *mode = NULL;
    char *color = NULL;
    char *back_ground = NULL;

    int i = 0;
    while (mode_color[i].flag != 0) {
        if (mode_f == mode_color[i].flag) {
            mode = mode_color[i].color_mode;
            break;
        }
        ++i;
    }

    if (mode_color[i].flag ==0) {
        mode = "0";
    }
    i = 0;

    while (front_color[i].flag != 0) {
        if (color_f == front_color[i].flag) {
            color = front_color[i].color_mode;
            break;
        }
        ++i;
    }
    if (front_color[i].flag == 0) {
        color = "37";
    }
    i = 0;

    while (bground_color[i].flag != 0) {
        if (bground_f == bground_color[i].flag) {
            back_ground = bground_color[i].color_mode;
            break;
        }
        ++i;
    }
    if (bground_color[i].flag == 0) {
        back_ground = "40";
    }

    char fmt_buf[1024] = {0};
    sprintf(fmt_buf, "\033[%s;%s;%sm%s\033[0m", mode, color,
            back_ground, fmt);

    vprintf(fmt_buf, ap);
}
