/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_STRING_H__
#define __GT_STRING_H__

#include "gt_lib.h"

#ifdef  __cplusplus
extern "C" {
#endif

void *gt_memset(void *ptr, int value, int num);

void *gt_memcpy(void *destination, const void *source, int num);

void *gt_memmove(void *destination, const void *source, int num);

int   gt_memcmp(const void *ptr1, const void *ptr2, int num);

#ifdef  __cplusplus
}
#endif

#endif  /* __GT_STRING_H__ */
