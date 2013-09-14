/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_SEMAPHORE_H__
#define __GT_SEMAPHORE_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    void        *sem;
    GT_MAGIC_t   magic;
} GtSemaphore_t;

GtSemaphore_t *GtSemaphore_New(int count);

int GtSemaphore_Free(GtSemaphore_t *semaphore);

int GtSemaphore_Lock(GtSemaphore_t *semaphore);

int GtSemaphore_Unlock(GtSemaphore_t *semaphore);

// TODO: WAIT POST

#ifdef __cplusplus
}
#endif
   
#endif /* __GT_SEMAPHORE_H__ */
