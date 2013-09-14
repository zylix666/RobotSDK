/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_semaphore.h"

#ifdef __cplusplus
extern "C"
{
#endif

static int gtSemaphoreIsInit = 0;

int GtSemaphore_InitLib() {
    if (gtSemaphoreIsInit == 1) {
        return GT_OK_INIT_ALREADY;
    }
    gtSemaphoreIsInit = 1;
    
    return GT_OK;
}
    
GtSemaphore_t *GtSemaphore_New(int count) {
    if (gtSemaphoreIsInit == 0) return NULL;
    if (count < 0) return NULL;

    GtSemaphore_t *semaphore = gt_calloc(1, sizeof(GtSemaphore_t));
    if (semaphore == NULL) {
        return NULL;
    }
    
    int ret = sem_init((sem_t *)&semaphore->sem, 0, count);
    if (ret != 0) {
        gt_free(semaphore);
        return NULL;
    }

    semaphore->magic = GT_MAGIC_SEMAPHORE;

    return semaphore;
}

int GtSemaphore_Free(GtSemaphore_t *semaphore) {
    if (semaphore == NULL) return GT_ERROR_NULL_POINTER;
    if (semaphore->magic != GT_MAGIC_SEMAPHORE) return GT_ERROR_MAGIC;

    semaphore->magic = GT_MAGIC_UNKNOWN;
    
    sem_destroy((sem_t *)&semaphore->sem);
    
    gt_free(semaphore);
    
    return GT_OK;
}

int GtSemaphore_Lock(GtSemaphore_t *semaphore) {
    if (semaphore == NULL) return GT_ERROR_NULL_POINTER;
    if (semaphore->magic != GT_MAGIC_SEMAPHORE) return GT_ERROR_MAGIC;

    sem_wait((sem_t *)&semaphore->sem);
    
    return GT_OK;
}
   
int GtSemaphore_Unlock(GtSemaphore_t *semaphore) {
    if (semaphore == NULL) return GT_ERROR_NULL_POINTER;
    if (semaphore->magic != GT_MAGIC_SEMAPHORE) return GT_ERROR_MAGIC;

    sem_post((sem_t *)&semaphore->sem);
    
    return GT_OK;
}

#ifdef __cplusplus
}
#endif
