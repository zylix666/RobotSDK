/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_IMAGE_BARCODE_H__
#define __GT_IMAGE_BARCODE_H__

#include "gt_lib.h"

#include "gt_image.h"

#ifdef __cplusplus
extern "C" 
{
#endif

int GtImageBarcode_InitLib();

int GtImageBarcode_Decode(GtImage_t *image, gt_utf8 *code, int codeSize);

#ifdef __cplusplus
}
#endif

#endif /* __GT_IMAGE_BARCODE_H__ */
