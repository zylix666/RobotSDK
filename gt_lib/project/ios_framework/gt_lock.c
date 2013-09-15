/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_lock.h"

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

static int gtLockIsInit = 0;

int GtLock_InitLib() {
    if (gtLockIsInit == 1) {
        return GT_OK_INIT_ALREADY;
    }
    gtLockIsInit = 1;
    
    return GT_OK;
}

GtLock_t *GtLock_New() {
    if (gtLockIsInit == 0) return NULL;

    GtLock_t *lock = gt_calloc(1, sizeof(GtLock_t));
    if (lock == NULL) {
        return NULL;
    }
    
    lock->mutex = gt_calloc(1, sizeof(pthread_mutex_t));
    if (lock->mutex == NULL) {
        return NULL;
    }
    
    int ret = pthread_mutex_init((pthread_mutex_t *)lock->mutex, NULL);
    if (ret != 0) {
        gt_free(lock->mutex);
        gt_free(lock);
        return NULL;
    }

    lock->magic = GT_MAGIC_LOCK;

    return lock;
}

int GtLock_Free(GtLock_t *lock) {
    if (lock == NULL) return GT_ERROR_NULL_POINTER;
    if (lock->magic != GT_MAGIC_LOCK) return GT_ERROR_MAGIC;

    lock->magic = GT_MAGIC_UNKNOWN;
    
    pthread_mutex_destroy((pthread_mutex_t *)lock->mutex);

    gt_free(lock->mutex);
    
    gt_free(lock);

    return GT_OK;
}

int GtLock_Lock(GtLock_t *lock) {
    if (lock == NULL) return GT_ERROR_NULL_POINTER;
    if (lock->magic != GT_MAGIC_LOCK) return GT_ERROR_MAGIC;

    pthread_mutex_lock((pthread_mutex_t *)lock->mutex);
    
    return GT_OK;
}

int GtLock_Unlock(GtLock_t *lock) {
    if (lock == NULL) return GT_ERROR_NULL_POINTER;
    if (lock->magic != GT_MAGIC_LOCK) return GT_ERROR_MAGIC;

    pthread_mutex_unlock((pthread_mutex_t *)lock->mutex);
    
    return GT_OK;
}

#ifdef __cplusplus
}
#endif
