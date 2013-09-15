/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_image.h"
#include "gt_image_mt.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageMtIsInit = 0;

int GtImageMt_InitLib() {
	if (gtImageMtIsInit == 1) return GT_OK_INIT_ALREADY;
	gtImageMtIsInit = 1;
	
    return GT_OK;
}

int GtImageMt_Test(GtImage_t *image) {
	if (gtImageMtIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
