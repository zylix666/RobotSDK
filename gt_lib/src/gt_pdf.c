/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include <gt_pdf.h>

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtPdfIsInit = 0;

int GtPdf_InitLib(gt_utf8 *license, gt_utf8 *token) {
	if (gtPdfIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtPdfIsInit = 1;
	
    return GT_OK;
}

GtPdf_t *GtPdf_New() {
	GtPdf_t *gtPdf;

	if (gtPdfIsInit == 0) return NULL;

	gtPdf = (GtPdf_t *)gt_calloc(1, sizeof(GtPdf_t));
	if (gtPdf == NULL) {
		return NULL;
	}

	gtPdf->magic = GT_MAGIC_FILE;

	return gtPdf;
}

int GtPdf_Free(GtPdf_t *gtPdf) {
	if (gtPdf == NULL) return GT_ERROR_NULL_POINTER;
	if (gtPdf->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	gtPdf->magic = GT_MAGIC_UNKNOWN;

	gt_free(gtPdf);

	return GT_OK;
}

int GtPdf_Lock(GtPdf_t *gtPdf) {
	if (gtPdf == NULL) return GT_ERROR_NULL_POINTER;
	if (gtPdf->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	if (gtPdf->lock == NULL) {
		gtPdf->lock  = GtLock_New();
		if (gtPdf->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(gtPdf->lock);
}

int GtPdf_Unlock(GtPdf_t *gtPdf) {
	if (gtPdf== NULL) return GT_ERROR_NULL_POINTER;
	if (gtPdf->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	return GtLock_Unlock(gtPdf->lock);
}

int GtPdf_Open(GtPdf_t *gtPdf, gt_utf8 *path , int flags) {
	if (gtPdf == NULL) return GT_ERROR_PARAMETER_0;
	if (gtPdf->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;
	if (path == NULL) return GT_ERROR_PARAMETER_1;

	return GT_OK;
}

int GtPdf_Close(GtPdf_t *gtPdf) {
	if (gtPdf == NULL) return GT_ERROR_NULL_POINTER;
	if (gtPdf->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
