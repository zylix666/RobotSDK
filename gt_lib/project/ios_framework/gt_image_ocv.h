/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_IMAGE_OCV_H__
#define __GT_IMAGE_OCV_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

int GtImageOcv_InitLib();

int GtImageOcv_Test(GtImage_t *image);

int GtImageOcv_SetThreshold(GtImage_t *image, int threshold);

#ifdef __cplusplus
}
#endif
  
#endif /* __GT_IMAGE_OCV_H__ */
