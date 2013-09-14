/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_STACK_H__
#define __GT_STACK_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
	gt_utf8      elementType[4096];
    void        *data;
    void        *lock;
	int top;
	gt_size     elementSize, reservedLength;
    GT_MAGIC_t   magic;
} GtStack_t;

int GtStack_InitLib();

GtStack_t *GtStack_New();

int GtStack_Free(GtStack_t *stack);

int GtStack_Lock(GtStack_t *stack);

int GtStack_Unlock(GtStack_t *stack);

int GtStack_Test(GtStack_t *stack);

int GtStack_Init(GtStack_t *gtStack, gt_utf8 *elementType, gt_size elementSize, gt_size reservedLength);

gt_utf8 *GtStack_GetElementType(GtStack_t *gtStack);

int GtStack_GetElementSize(GtStack_t *gtStack);

int GtStack_Push(GtStack_t *gtStack, void *element);

int GtStack_Pop(GtStack_t *gtStack, void *element);

int GtStack_Empty(GtStack_t *gtStack);

int GtStack_GetLength(GtStack_t *gtStack);

#ifdef __cplusplus
}
#endif

#endif /* __GT_STACK_H__ */
