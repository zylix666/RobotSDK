/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_sound.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtSoundIsInit = 0;

int GtSound_InitLib() {
	if (gtSoundIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtSoundIsInit = 1;

	return GT_OK;
}

GtSound_t *GtSound_New() {
	if (gtSoundIsInit == 0) return NULL;
	
	GtSound_t *sound = (GtSound_t *)gt_calloc(1, sizeof(GtSound_t));
	if (sound == NULL) {
		return NULL;
	}

	sound->data = gt_calloc(1, sizeof(int));
	if (sound->data == NULL) {
		gt_free(sound);
		return NULL;
	}
	
	sound->magic = GT_MAGIC_SOUND;

	return sound;
}

int GtSound_Free(GtSound_t *sound) {
	if (sound == NULL) return GT_ERROR_NULL_POINTER;
	if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	sound->magic = GT_MAGIC_UNKNOWN;

	gt_free(sound->data);
	
    GtLock_Free((GtLock_t *)sound->lock);
    
    gt_free(sound);
    
    return GT_OK;
}

int GtSound_Lock(GtSound_t *sound) {
	if (sound == NULL) return GT_ERROR_NULL_POINTER;
	if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	if (sound->lock == NULL) {
		sound->lock = GtLock_New();
		if (sound->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)sound->lock);
}

int GtSound_Unlock(GtSound_t *sound) {
	if (sound == NULL) return GT_ERROR_NULL_POINTER;
	if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)sound->lock);
}

int GtSound_Test(GtSound_t *sound) {
	if (sound == NULL) return GT_ERROR_NULL_POINTER;
	if (sound->magic != GT_MAGIC_SOUND) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
