/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_TIMER_US_H__
#define __GT_TIMER_US_H__

#include "gt_lib.h"

typedef struct {
	gt_int64     start;
	gt_int64     end;

	void        *lock;
	GT_MAGIC_t   magic;
} GtTimerUs_t;

#ifdef __cplusplus
extern "C" 
{
#endif

int GtTimerUs_InitLib();

GtTimerUs_t *GtTimerUs_New();

int GtTimerUs_Free(GtTimerUs_t *timerUs);

int GtTimerUs_Lock(GtTimerUs_t *timerUs);

int GtTimerUs_Unlock(GtTimerUs_t *timerUs);

gt_int64 GtTimerUs_GetTimeStamp();

int GtTimerUs_Start(GtTimerUs_t *timerUs);

int GtTimerUs_Stop(GtTimerUs_t *timerUs);

gt_int64 GtTimerUs_GetUs(GtTimerUs_t *timerUs);

#ifdef __cplusplus
}
#endif

#endif /* __GT_TIMER_MS_H__ */
