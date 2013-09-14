/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_IMAGE_CUDA_H__
#define __GT_IMAGE_CUDA_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

int GtImageCuda_InitLib(gt_utf8 *license, gt_utf8 *token);

int GtImageCuda_Test();

int GtImageCuda_SetThreshold(GtImage_t *image, int threshold);

int GtImageCuda_SetBrightness(GtImage_t *image, int brightness);

int GtImageCuda_GetCorrelation(GtImage_t *imageRoi, GtImage_t *imagePattern, GtImage_t *imageCorrelation);

int GtImageCuda_SetContrast(GtImage_t *image, float contrast);

int GtImageCuda_Invert(GtImage_t *image);

#ifdef __cplusplus
}
#endif

#endif /* __GT_IMAGE_CUDA_H__ */
