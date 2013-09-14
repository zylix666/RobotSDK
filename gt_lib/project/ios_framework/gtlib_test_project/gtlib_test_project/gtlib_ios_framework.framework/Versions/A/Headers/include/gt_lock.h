/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_LOCK_H__
#define __GT_LOCK_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    void *mutex;
    
    GT_MAGIC_t magic;
} GtLock_t;

GtLock_t *GtLock_New();

int GtLock_Free(GtLock_t *lock);

int GtLock_Lock(GtLock_t *lock);

int GtLock_Unlock(GtLock_t *lock);

#ifdef __cplusplus
}
#endif
    
#endif /* __GT_LOCK_H__ */
