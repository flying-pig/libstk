/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  test_string.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  test for stk string utils.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "stk_string.h"

int main(int argc, char *argv[])
{
  char src0[] = " aaaaaaa";
  char src1[] = "";
  char src2[] = "AaaaAbbbAcccA";
  char src3[] = "bbbbbb  ";
  char src4[] = "   aaaaaaa";
  char src5[] = "bbbbbb ";
  char src6[] = "   bbbbbb   ";

  char *dest;
  dest = strtrim_left(src0, ' ');
  printf("src0 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "aaaaaaa") == 0);
  
  dest = strtrim_left(src1, ' ');
  printf("src1 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "") == 0);

  dest = strtrim(src2, 'A');
  printf("src2 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "aaaAbbbAccc") == 0);

  dest = strtrim_right(src3, ' ');
  printf("src3 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "bbbbbb") == 0);

  dest = strtrim_left(src4, ' ');
  printf("str4 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "aaaaaaa") == 0);

  dest = strtrim_right(src5, ' ');
  printf("str5 trim ' ' dest:[%s]\n", dest);
  assert(strcmp(dest, "bbbbbb") == 0);

  dest = trim_right_m(src3);
  assert(strcmp(dest, "bbbbbb") == 0);

  char out[1024] = {0};
  trim(out, 1024, src6);
  printf("str6 trim ' ' out:[%s]\n", out);
  assert(strcmp(out, "bbbbbb") == 0);

  return 0;
}
