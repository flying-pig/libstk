/*
 *  Copyright (c) 2013 SINA Corporation, All Rights Reserved.
 *
 *  x_alloc.h:  Chen Huaying <chenhuaying@sina.cn>
 *
 *  x allocate.
 */

#ifndef __X_ALLOC_H
#define __X_ALLOC_H

#include <string.h>
#include <stdlib.h>

#define NGX_ALIGNMENT   sizeof(unsigned long)    /*  platform word */

#define x_memzero(buf, n)       (void) memset(buf, 0, n)
#define x_memset(buf, c, n)     (void) memset(buf, c, n)

void *x_alloc(size_t size);
void *x_calloc(size_t size);

#define x_free                  free

#endif /*__X_ALLOC_H*/
