/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_FIFO_H__
#define __GT_FIFO_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
	gt_utf8      elementType[4096];
    void        *data;
    void        *lock;
	int          elementSize;
	int          reservedLength;
	int fr,bk,length;
    GT_MAGIC_t   magic;
} GtFifo_t;

int GtFifo_InitLib();

GtFifo_t *GtFifo_New();

int GtFifo_Free(GtFifo_t *fifo);

int GtFifo_Lock(GtFifo_t *fifo);

int GtFifo_Unlock(GtFifo_t *fifo);

int GtFifo_Test(GtFifo_t *fifo);

int GtFifo_Init(GtFifo_t *gtFifo, gt_utf8 *elementType, int elementSize, int reservedLength);

gt_utf8 *GtFifo_GetElementType(GtFifo_t *gtFifo);

int GtFifo_GetElementSize(GtFifo_t *gtFifo);

int GtFifo_Push(GtFifo_t *gtFifo, void *element);

int GtFifo_Pop(GtFifo_t *gtFifo, void *element);

int GtFifo_Empty(GtFifo_t *gtFifo);

int GtFifo_GetLength(GtFifo_t *gtFifo);

#ifdef __cplusplus
}
#endif

#endif /* __GT_FIFO_H__ */
