/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  x_alloc.c:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  x allocate.
 */

#include <stdlib.h>
#include "x_alloc.h"

void *x_alloc(size_t size)
{
  void *p;
  p = malloc(size);
  return p;
}

void *x_calloc(size_t size)
{
  void *p;
  p = malloc(size);
  if (p) {
    x_memzero(p, size);
  }
  return p;
}
