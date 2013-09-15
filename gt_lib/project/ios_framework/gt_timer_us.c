/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_timer_us.h"

#include <time.h>
#include <sys/time.h>

static int gtTimerUsIsInit = 0;

#ifdef __cplusplus
extern "C" 
{
#endif

int GtTimerUs_InitLib() {
	if (gtTimerUsIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtTimerUsIsInit = 1;

	return GT_OK;
}

GtTimerUs_t *GtTimerUs_New() {
	if (gtTimerUsIsInit == 0) return NULL;
	
	GtTimerUs_t *timerUs = (GtTimerUs_t *)gt_calloc(1, sizeof(GtTimerUs_t));
	if (timerUs == NULL) {
		return NULL;
	}
	
	timerUs->magic = GT_MAGIC_TIMER_US;

	return timerUs;
}

int GtTimerUs_Free(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	timerUs->magic = GT_MAGIC_UNKNOWN;
	
    GtLock_Free(timerUs->lock);
    
    gt_free(timerUs);
    
    return GT_OK;
}

int GtTimerUs_Lock(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	if (timerUs->lock == NULL) {
		timerUs->lock = GtLock_New();
		if (timerUs->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(timerUs->lock);
}

int GtTimerUs_Unlock(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	return GtLock_Unlock(timerUs->lock);
}

gt_int64 GtTimerUs_GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (gt_int64)(tv.tv_sec * (gt_uint64)1000000 + tv.tv_usec);
}

int GtTimerUs_Start(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	timerUs->start = GtTimerUs_GetTimeStamp();

	return GT_OK;
}

int GtTimerUs_Stop(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	timerUs->end = GtTimerUs_GetTimeStamp();

	return GT_OK;
}

gt_int64 GtTimerUs_GetUs(GtTimerUs_t *timerUs) {
	if (timerUs == NULL) return GT_ERROR_NULL_POINTER;
	if (timerUs->magic != GT_MAGIC_TIMER_US) return GT_ERROR_MAGIC;

	return timerUs->end - timerUs->start;
}

#ifdef __cplusplus
}
#endif
