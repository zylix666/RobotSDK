/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_face_detection.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtFaceDetectionIsInit = 0;

int GtFaceDetection_InitLib() {
	if (gtFaceDetectionIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtFaceDetectionIsInit = 1;

	return GT_OK;
}

GtFaceDetection_t *GtFaceDetection_New() {
	GtFaceDetection_t *faceDetection;

	if (gtFaceDetectionIsInit == 0) return NULL;
	
	faceDetection = (GtFaceDetection_t *)gt_calloc(1, sizeof(GtFaceDetection_t));
	if (faceDetection == NULL) {
		return NULL;
	}

	faceDetection->data = gt_calloc(1, sizeof(int));
	if (faceDetection->data == NULL) {
		gt_free(faceDetection);
		return NULL;
	}
	
	faceDetection->magic = GT_MAGIC_TEMPLATE;

	return faceDetection;
}

int GtFaceDetection_Free(GtFaceDetection_t *faceDetection) {
	if (faceDetection == NULL) return GT_ERROR_NULL_POINTER;
	if (faceDetection->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	faceDetection->magic = GT_MAGIC_UNKNOWN;

	gt_free(faceDetection->data);
	
    GtLock_Free((GtLock_t *)faceDetection->lock);
    
    gt_free(faceDetection);
    
    return GT_OK;
}

int GtFaceDetection_Lock(GtFaceDetection_t *faceDetection) {
	if (faceDetection == NULL) return GT_ERROR_NULL_POINTER;
	if (faceDetection->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	if (faceDetection->lock == NULL) {
		faceDetection->lock = GtLock_New();
		if (faceDetection->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)faceDetection->lock);
}

int GtFaceDetection_Unlock(GtFaceDetection_t *faceDetection) {
	if (faceDetection == NULL) return GT_ERROR_NULL_POINTER;
	if (faceDetection->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)faceDetection->lock);
}

int GtFaceDetection_Test(GtFaceDetection_t *faceDetection) {
	if (faceDetection == NULL) return GT_ERROR_NULL_POINTER;
	if (faceDetection->magic != GT_MAGIC_TEMPLATE) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
