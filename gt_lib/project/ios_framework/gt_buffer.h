/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_BUFFER_H__
#define __GT_BUFFER_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    gt_byte *buf;
	int size;
    int length;
    int align;

    void        *lock;
    GT_MAGIC_t   magic;
} GtBuffer_t;

GtBuffer_t *GtBuffer_New(int size, int align);

int GtBuffer_Free(GtBuffer_t *buffer);

int GtBuffer_Lock(GtBuffer_t *buffer);

int GtBuffer_Unlock(GtBuffer_t *buffer);

int GtBuffer_SetSize(GtBuffer_t *buffer, int size);

int GtBuffer_Append(GtBuffer_t *buffer, gt_byte *buf, int length);

int GtBuffer_GetLength(GtBuffer_t *buffer);

int GtBuffer_GetSize(GtBuffer_t *buffer);

int GtBuffer_Get(GtBuffer_t *buffer, gt_byte *buf, int length);

int GtBuffer_Empty(GtBuffer_t *buffer);

#ifdef __cplusplus
}
#endif
    
#endif /* __GT_BUFFER_H__ */
