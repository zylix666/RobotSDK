/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_IMAGE_H__
#define __GT_IMAGE_H__

#include "gt_lib.h"

typedef struct {
  int      x;
  int      y;
  gt_uint8 gray8;
} GtImageBlobPoint_t;

typedef struct {
  int id;
  int count;
} GtImageBlobCluster_t;

typedef struct {
	GT_COLOR_MODE_t   colorMode;
	gt_int            width;
	gt_int            height;
	gt_byte          *data;
	gt_int            dataSize;

	// Gabor filter.
	gt_int    gabor_width;
	gt_int    gabor_height;
	gt_int    gabor_dir_no;
	gt_int    gabor_block_size;
	gt_int    gabor_dir_avg_size;
	gt_int    gabor_filter_size;
	gt_double gabor_freq;
	gt_double gabor_delta_x;
	gt_double gabor_delta_y;
	gt_int ***gabor_coef;
	gt_int  **gabor_pp_i_data;   // Orientation
	gt_int  **gabor_pp_i_buf;
	gt_byte **gabor_pp_b_data;
	gt_byte **gabor_pp_b_buf;

	GtLock_t         *lock;
	GT_MAGIC_t        magic;
} GtImage_t;

#ifdef __cplusplus
extern "C"
{
#endif

int GtImage_InitLib();

GtImage_t *GtImage_New();

int GtImage_Free(GtImage_t *image);

int GtImage_Lock(GtImage_t *image);

int GtImage_Unlock(GtImage_t *image);

int GtImage_InitImage(GtImage_t *image, int width, int height, int colorMode);

int GtImage_GetColorMode(GtImage_t *image);

int GtImage_SetColorMode(GtImage_t *image, int colorMode);

int GtImage_Fill(GtImage_t *image, GtColor_t *color);

int GtImage_GetPixelColor(GtImage_t *image, int x, int y, GtColor_t *color);

int GtImage_DrawPixel(GtImage_t *image, int x, int y, GtColor_t *color);

int GtImage_SetThreshold(GtImage_t *image, int threshold);

int GtImage_SetBrightness(GtImage_t *image, int brightness);

int GtImage_GetCorrelation(GtImage_t *imageRoi, GtImage_t *imagePattern, GtImage_t *imageCorrelation);

int GtImage_Load(GtImage_t *image, gt_byte *buffer, int bufferLength, int colorMode, int width, int height);

int GtImage_Save(GtImage_t *image, gt_byte *buffer, int bufferLength, int colorMode, int width, int height);

int GtImage_SetContrast(GtImage_t *image, float contrast);

int GtImage_Invert(GtImage_t *image);

int GtImage_GetDepthImage(GtImage_t *imageDepth, GtImage_t *imageLeft, GtImage_t *imageRight);

int GtImage_Blob(GtImage_t *imageBlob, GtImage_t *imageSrc, int range);

int GtImage_ConvertBlobToGray8(GtImage_t *imageDst, GtImage_t *imageBlob);

#ifdef __cplusplus
}
#endif
    
#endif /* __GT_IMAGE_H__ */
