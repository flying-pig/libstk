/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  test_ae.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  test for ae event driven library.
 */
#include <stdlib.h>
#include <stdio.h>

#include "ae.h"

static int max_loop = 10;
static int loop_count = 0;

int time_proc(aeEventLoop *event_loop, long long id, void *data)
{
  ++loop_count;
  printf("Time Event: id(%lld) data(%s)\n", id, (char *)data);

  if (loop_count > max_loop) return -1;
  return 2000;
}

void final_proc(aeEventLoop *event_loop, void *data)
{
  printf("Final Time Event: data(%s)\n", (char *)data);
}

int main(int argc, char *argv[])
{
  aeEventLoop *main_loop = aeCreateEventLoop(10240);
  long long id = aeCreateTimeEvent(main_loop, 1500, time_proc, "time alarm", final_proc);
  printf("main loop add id(%lld) time event\n", id);
  aeMain(main_loop);
  aeDeleteEventLoop(main_loop);
  return 0;
}
