/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  stk_setproctitle.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  set process title.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stk_comm.h"
#include "stk_string.h"
#include "x_alloc.h"
#include "stk_log.h"

extern char **environ;
static char *stk_argv_last;
static char **stk_argv;

int stk_init_setproctitle(int argc, char **argv)
{
  u_char   *p;
  size_t    size;
  u_int     i;

  size = 0;

  for (i = 0; environ[i]; i++) {
    size += strlen(environ[i]) + 1;
  }

  p = x_alloc(size); 
  if (p == NULL) {
    return STK_ERROR;
  }

  stk_argv_last = argv[0];

  for (i = 0; argv[i]; i++) {
    if (stk_argv_last == argv[i]) {
      stk_argv_last = argv[i] + strlen(argv[i]) + 1;
    }
  }

  for (i = 0; environ[i]; i++) {
    if (stk_argv_last == environ[i]) {

      size = strlen(environ[i]) + 1;
      stk_argv_last = environ[i] + size;

      stk_cpystrn(p, (u_char *) environ[i], size);
      environ[i] = (char *) p;
      p += size;
    }
  }

  stk_argv_last--;
  stk_argv = argv;

  return STK_OK;
}

void stk_setproctitle(char *title)
{
  u_char *p;

  stk_argv[1] = NULL;

  p = stk_cpystrn((u_char *) stk_argv[0], (u_char *) title,
                  stk_argv_last - stk_argv[0]);

  if (stk_argv_last - (char *) p) {
    memset(p, 0, stk_argv_last - (char *) p);
  }

  stk_log_debug("setproctitle: \"%s\"", stk_argv[0]);

}

void stk_setproctitle2(char *prog, char *title)
{
  u_char *p;

  stk_argv[1] = NULL;

  p = stk_cpystrn((u_char *) stk_argv[0], (u_char *) prog,
                  stk_argv_last - stk_argv[0]);

  p = stk_cpystrn(p, (u_char *) title, stk_argv_last - (char *) p);

  if (stk_argv_last - (char *) p) {
    memset(p, 0, stk_argv_last - (char *) p);
  }

  stk_log_debug("setproctitle: \"%s\"", stk_argv[0]);
}
