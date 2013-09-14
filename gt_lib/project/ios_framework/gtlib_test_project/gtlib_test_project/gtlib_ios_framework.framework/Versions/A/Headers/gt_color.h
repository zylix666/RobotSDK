/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_COLOR_H__
#define __GT_COLOR_H__

#include "gt_lib.h"

typedef enum {
	GT_COLOR_MODE_UNKNOWN   = 0,
	GT_COLOR_MODE_RGB888    = 1,
	GT_COLOR_MODE_GRAY8     = 2,
	GT_COLOR_MODE_RGB565    = 3,
	GT_COLOR_MODE_ARGB8888  = 4,
	GT_COLOR_MODE_ARGB4444  = 5,
	GT_COLOR_MODE_INT32     = 6,
	GT_COLOR_MODE_FLOAT32   = 7,
	GT_COLOR_MODE_DOUBLE64  = 8,
	// GT_COLOR_MODE_BLOB,  
	// GT_COLOR_MODE_YCbCr888,
	// GT_COLOR_MODE_ARGB8888,  
	// GT_COLOR_MODE_RGB565,
	// GT_COLOR_MODE_ARGB1555,
	// GT_COLOR_MODE_ARGB4444,
	// GT_COLOR_MODE_ARGB5676,
	// GT_COLOR_MODE_ARGB8565,
	// GT_COLOR_MODE_YUY2,            // 16-bit 4:2:2.
	// GT_COLOR_MODE_IMC2,            // 12-bit 4:2:0.
	// GT_COLOR_MODE_IMC4,            // 12-bit 4:2:0.
	// GT_COLOR_MODE_YV12,            // 12-bit 4:2:0.
	// GT_COLOR_MODE_iYUV,            // 12-bit 4:2:0.
	// GT_COLOR_MODE_NV12,            // 12-bit 4:2:0.
	// GT_COLOR_MODE_IMC1,            // 16-bit 4:2:0.
	// GT_COLOR_MODE_IMC3,            // 16-bit 4:2:0.
	// GT_COLOR_MODE_UYVY,            // 16-bit 4:2:2.
	// GT_COLOR_MODE_AYUV,            // 32-bit 4:4:4.
	// GT_COLOR_MODE_YUV,
	GT_COLOR_MODE_COUNT
} GT_COLOR_MODE_t;

typedef struct {
	GT_COLOR_MODE_t  mode;

	gt_uint8         red8;
	gt_uint8         green8;
	gt_uint8         blue8;
	gt_uint8         alpha8;

	gt_uint8         gray8;

	gt_uint8         y8;
	gt_uint8         cb8;
	gt_uint8         cr8;

	gt_uint16        red16;
	gt_uint16        green16;
	gt_uint16        blue16;

	gt_uint16        gray16;

	gt_int32         int32;
	gt_float32       float32;
	gt_double        double64;

	void       *lock;
	GT_MAGIC_t  magic;
} GtColor_t;  

#ifdef __cplusplus
extern "C"
{
#endif

GtColor_t *GtColor_New();

int GtColor_Free(GtColor_t *color);

int GtColor_SetRGB888(GtColor_t *color, int red8, int green8, int blue8);

int GtColor_SetGRAY8(GtColor_t *color, int gray8);

GT_COLOR_MODE_t GtColor_GetMode(GtColor_t *color);

int GtColor_GetRed8(GtColor_t *color);

int GtColor_GetGreen8(GtColor_t *color);

int GtColor_GetBlue8(GtColor_t *color);

int GtColor_GetGray8(GtColor_t *color);

#ifdef __cplusplus
}
#endif
    
#endif /* __GT_COLOR_H__ */
