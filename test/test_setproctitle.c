/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  test_setproctitle.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  test setproctitle.
 */
#include <stdlib.h>
#include <stdio.h>
#include "stk_setproctitle.h"

extern char **environ;

int main(int argc, char *argv[])
{
  int i;
  printf("argc: %d\n", argc);
  for (i = 0; i < argc; i++) {
    printf("%p\n", argv[i]);
    printf("argv[%d]: %s\n", i, argv[i]);
  }

  for (i = 0; environ[i]; i++) {
    printf("%p\n", environ[i]);
    printf("environ[%d]: %s\n", i, environ[i]);
  }

  stk_init_setproctitle(argc, argv);

  stk_setproctitle("new proc title");

  puts("#############################################");

  for (i = 0; i < argc; i++) {
    printf("%p\n", argv[i]);
    printf("argv[%d]: %s\n", i, argv[i]);
  }

  for (i = 0; environ[i]; i++) {
    printf("%p\n", environ[i]);
    printf("environ[%d]: %s\n", i, environ[i]);
  }
  return 0;
}
