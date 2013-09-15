/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_pid.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtPidIsInit = 0;

int GtPid_InitLib() {
	if (gtPidIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtPidIsInit = 1;

	return GT_OK;
}

GtPid_t *GtPid_New() {
	GtPid_t *pid;

	if (gtPidIsInit == 0) return NULL;
	
	pid = (GtPid_t *)gt_calloc(1, sizeof(GtPid_t));
	if (pid == NULL) {
		return NULL;
	}

	pid->data = gt_calloc(1, sizeof(int));
	if (pid->data == NULL) {
		gt_free(pid);
		return NULL;
	}
	
	pid->magic = GT_MAGIC_PID;

	return pid;
}

int GtPid_Free(GtPid_t *pid) {
	if (pid == NULL) return GT_ERROR_NULL_POINTER;
	if (pid->magic != GT_MAGIC_PID) return GT_ERROR_MAGIC;

	pid->magic = GT_MAGIC_UNKNOWN;

	gt_free(pid->data);
	
    GtLock_Free((GtLock_t *)pid->lock);
    
    gt_free(pid);
    
    return GT_OK;
}

int GtPid_Lock(GtPid_t *pid) {
	if (pid == NULL) return GT_ERROR_NULL_POINTER;
	if (pid->magic != GT_MAGIC_PID) return GT_ERROR_MAGIC;

	if (pid->lock == NULL) {
		pid->lock = GtLock_New();
		if (pid->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)pid->lock);
}

int GtPid_Unlock(GtPid_t *pid) {
	if (pid == NULL) return GT_ERROR_NULL_POINTER;
	if (pid->magic != GT_MAGIC_PID) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)pid->lock);
}

int GtPid_Test(GtPid_t *pid) {
	if (pid == NULL) return GT_ERROR_NULL_POINTER;
	if (pid->magic != GT_MAGIC_PID) return GT_ERROR_MAGIC;

	return GT_OK;
}

double GtPid_Pid(GtPid_t *pid, double feedback, double desire, double p, double i, double d, double timeSec) {
	double error;
	
	if (pid == NULL) return GT_ERROR_PARAMETER_0;
	if (pid->magic != GT_MAGIC_PID) return GT_ERROR_PARAMETER_0;

	error = desire - feedback;
	//GT_LOGI("error = %f",error );

	feedback = p * error + i * (error + pid->errorOld) * timeSec / 2 + d * (error - pid->errorOld)/timeSec;
	//GT_LOGI("feedback = %f",feedback );
	pid->errorOld= error;

	return feedback;
}

#ifdef __cplusplus
}
#endif
