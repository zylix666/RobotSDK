/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_ARRAY_H__
#define __GT_ARRAY_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
	gt_utf8      elementType[4096];
	int          elementSize;
	int          length;
	int          realLength;
	int          reservedLength;
	void        *data;
    void        *lock;
    GT_MAGIC_t   magic;
} GtArray_t;

int GtArray_InitLib();

GtArray_t *GtArray_New();

int GtArray_Free(GtArray_t *array);

int GtArray_Lock(GtArray_t *array);

int GtArray_Unlock(GtArray_t *array);

int GtArray_Test(GtArray_t *array);

int GtArray_Init(GtArray_t *gtArray, gt_utf8 *elementType, gt_size elementSize, gt_size reservedLength);

gt_utf8 *GtArray_GetElementType(GtArray_t *gtArray);

int GtArray_GetElementSize(GtArray_t *gtArray);

void *GtArray_Get(GtArray_t *gtArray, int index);

int GtArray_Copy(GtArray_t *gtArray, int index, void *element);

int GtArray_Set(GtArray_t *gtArray, int index, void *element);

int GtArray_Append(GtArray_t *gtArray, void *element);

int GtArray_Insert(GtArray_t *gtArray, int index, void *element);

int GtArray_Remove(GtArray_t *gtArray, int index);

int GtArray_SetReservedLength(GtArray_t *gtArray, int reservedLength);

int GtArray_SetRealLength(GtArray_t *gtArray, int realLength);

int GtArray_SetLength(GtArray_t *gtArray, int length);

int GtArray_GetRealLength(GtArray_t *gtArray);

int GtArray_GetReservedLength(GtArray_t *gtArray);

int GtArray_GetLength(GtArray_t *gtArray);

int GtArray_CheckElementType(GtArray_t *gtArray, gt_utf8 *elementType);

#ifdef __cplusplus
}
#endif

#endif /* __GT_ARRAY_H__ */
