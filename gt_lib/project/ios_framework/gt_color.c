/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_color.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtColorIsInit = 0;

int GtColor_InitLib() {
	if (gtColorIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtColorIsInit = 1;
	
    return GT_OK;
}

GtColor_t *GtColor_New() {
	if (gtColorIsInit == 0) return NULL;

	GtColor_t *color = (GtColor_t *)gt_calloc(1, sizeof(GtColor_t));
	if (color == NULL) {
		return NULL;
	}

	color->magic = GT_MAGIC_COLOR;

	return color;
}

int GtColor_Free(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;

	color->magic = GT_MAGIC_UNKNOWN;

	gt_free(color);

	return GT_OK;
}

int GtColor_Lock(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;

	if (color->lock == NULL) {
		color->lock  = GtLock_New();
		if (color->lock  == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(color->lock);
}

int GtColor_Unlock(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;

	return GtLock_Unlock(color->lock);
}

int GtColor_SetRGB888(GtColor_t *color, int red8, int green8, int blue8) {
	if (color == NULL) return GT_ERROR_PARAMETER_0;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_PARAMETER_0;
	if (red8 < 0) return GT_ERROR_PARAMETER_1;
	if (red8 > 255) return GT_ERROR_PARAMETER_1;
	if (green8 < 0) return GT_ERROR_PARAMETER_2;
	if (green8 > 255) return GT_ERROR_PARAMETER_2;
	if (blue8 < 0) return GT_ERROR_PARAMETER_3;
	if (blue8 > 255) return GT_ERROR_PARAMETER_3;

	color->mode = GT_COLOR_MODE_RGB888;
	color->red8 = red8;
	color->green8 = green8;
	color->blue8 = blue8;

	return GT_OK;
}

int GtColor_SetGRAY8(GtColor_t *color, int gray8) {
	if (color == NULL) return GT_ERROR_PARAMETER_0;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_PARAMETER_0;
	if (gray8 < 0) return GT_ERROR_PARAMETER_1;
	if (gray8 > 255) return GT_ERROR_PARAMETER_1;

	color->mode = GT_COLOR_MODE_GRAY8;
	color->gray8 = gray8;

	return GT_OK;
}

GT_COLOR_MODE_t GtColor_GetMode(GtColor_t *color) {
	if (color == NULL) return GT_COLOR_MODE_UNKNOWN;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;

	return color->mode;
}

int GtColor_GetRed8(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;
	if (color->mode != GT_COLOR_MODE_RGB888) return GT_ERROR_COLOR_MODE;

	return color->red8;
}

int GtColor_GetGreen8(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;
	if (color->mode != GT_COLOR_MODE_RGB888) return GT_ERROR_COLOR_MODE;

	return color->green8;
}

int GtColor_GetBlue8(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;
	if (color->mode != GT_COLOR_MODE_RGB888) return GT_ERROR_COLOR_MODE;

	return color->blue8;
}

int GtColor_GetGray8(GtColor_t *color) {
	if (color == NULL) return GT_ERROR_NULL_POINTER;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_MAGIC;
	if (color->mode != GT_COLOR_MODE_GRAY8) return GT_ERROR_COLOR_MODE;

	return color->gray8;
}

#ifdef __cplusplus
}
#endif
