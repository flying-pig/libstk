/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  test_bits.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  test bits.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stk_comm.h"

int main(int argc, char *argv[])
{
	int a = 0x0;
	setbit(a, 1);
	assert(a == 0x2);

	a = 0x0;
	setbit(a, 10);
	assert(a == 1024);

	clrbit(a, 10);
	assert(a == 0);

	a = 0x9;
	revbit(a, 3);
	assert(a == 0x1);
	return 0;
}
