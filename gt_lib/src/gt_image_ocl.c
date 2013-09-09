/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_image_ocl.h"
#include "gt_image.h"

#if defined(GT_CONFIG_OPEN_CL)
    #ifdef __APPLE__
        #include <OpenCL/opencl.h>
    #else
        #include <CL/cl.h>
    #endif
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageOclIsInit = 0;

int GtImageOcl_InitLib() {
	if (gtImageOclIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtImageOclIsInit = 1;
	
    return GT_OK;
}

int GtImageOcl_Test(GtImage_t *image) {
	if (gtImageOclIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
