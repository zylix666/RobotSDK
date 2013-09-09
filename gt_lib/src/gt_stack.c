/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_stack.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtStackIsInit = 0;

int GtStack_InitLib(gt_utf8 *license, gt_utf8 *token) {
	if (gtStackIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtStackIsInit = 1;

	return GT_OK;
}

GtStack_t *GtStack_New() {
	if (gtStackIsInit == 0) return NULL;
	
	GtStack_t *stack = (GtStack_t *)gt_calloc(1, sizeof(GtStack_t));
	if (stack == NULL) {
		return NULL;
	}

	stack->data = gt_calloc(1, sizeof(int));
	if (stack->data == NULL) {
		gt_free(stack);
		return NULL;
	}
	
	stack->magic = GT_MAGIC_STACK;

	return stack;
}

int GtStack_Free(GtStack_t *stack) {
	if (stack == NULL) return GT_ERROR_NULL_POINTER;
	if (stack->magic != GT_MAGIC_STACK) return GT_ERROR_MAGIC;

	stack->magic = GT_MAGIC_UNKNOWN;

	gt_free(stack->data);
	
    GtLock_Free((GtLock_t *)stack->lock);
    
    gt_free(stack);
    
    return GT_OK;
}

int GtStack_Lock(GtStack_t *stack) {
	if (stack == NULL) return GT_ERROR_NULL_POINTER;
	if (stack->magic != GT_MAGIC_STACK) return GT_ERROR_MAGIC;

	if (stack->lock == NULL) {
		stack->lock = GtLock_New();
		if (stack->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)stack->lock);
}

int GtStack_Unlock(GtStack_t *stack) {
	if (stack == NULL) return GT_ERROR_NULL_POINTER;
	if (stack->magic != GT_MAGIC_STACK) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)stack->lock);
}

int GtStack_Test(GtStack_t *stack) {
	if (stack == NULL) return GT_ERROR_NULL_POINTER;
	if (stack->magic != GT_MAGIC_STACK) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtStack_Init(GtStack_t *gtStack, gt_utf8 *elementType, gt_size elementSize, gt_size reservedLength)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	if(elementSize <= 0) return GT_ERROR_PARAMETER_2;
	if(reservedLength <= 0) return GT_ERROR_PARAMETER_3;
	
	gtStack->top = 0;
	gtStack->elementSize = elementSize;
	gtStack->reservedLength = reservedLength;
	gtStack->data = gt_calloc(reservedLength, elementSize);
	strcpy((char*)gtStack->elementType, (char*)elementType);

	return GT_OK;
}

gt_utf8 *GtStack_GetElementType(GtStack_t *gtStack)
{
	return gtStack->elementType;
}

int GtStack_GetElementSize(GtStack_t *gtStack)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	return gtStack->elementSize;
}

int GtStack_Push(GtStack_t *gtStack, void *element)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	if(gtStack->top >= gtStack->reservedLength)
	{
		//size not enough
		return -1;
	}

	gt_memcpy(gtStack->data + gtStack->elementSize * gtStack->top, element, gtStack->elementSize);
	gtStack->top++;
	return GT_OK;
}

int GtStack_Pop(GtStack_t *gtStack, void *element)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	if(gtStack->top == 0)
	{
		//pop nothing
		return -1;
	}

	gtStack->top--;
	gt_memcpy(element, gtStack->data + gtStack->elementSize * gtStack->top, gtStack->elementSize);
	return GT_OK;
}

int GtStack_Empty(GtStack_t *gtStack)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	return gtStack->top == 0;
}

int GtStack_GetLength(GtStack_t *gtStack)
{
	if(gtStack == NULL) return GT_ERROR_NULL_POINTER;
	return gtStack->top;
}

#ifdef __cplusplus
}
#endif
