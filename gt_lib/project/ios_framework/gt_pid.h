/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_PID_H__
#define __GT_PID_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    void        *data;

    double       errorOld;

    void        *lock;
    GT_MAGIC_t   magic;
} GtPid_t;

int GtPid_InitLib();

GtPid_t *GtPid_New();

int GtPid_Free(GtPid_t *pid);

int GtPid_Lock(GtPid_t *pid);

int GtPid_Unlock(GtPid_t *pid);

int GtPid_Test(GtPid_t *pid);

double GtPid_Pid(GtPid_t *pid, double feedback, double desire, double p, double i, double d, double timeSec);

#ifdef __cplusplus
}
#endif

#endif /* __GT_PID_H__ */
