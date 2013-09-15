/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_fifo.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtFifoIsInit = 0;

int GtFifo_InitLib(gt_utf8 *license, gt_utf8 *token) {


	if (gtFifoIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtFifoIsInit = 1;

	return GT_OK;
}

GtFifo_t *GtFifo_New() {

	GtFifo_t *fifo ;
	if (gtFifoIsInit == 0) return NULL;
	
	fifo = (GtFifo_t *)gt_calloc(1, sizeof(GtFifo_t));
	if (fifo == NULL) {
		return NULL;
	}

	fifo->data = gt_calloc(1, sizeof(int));
	if (fifo->data == NULL) {
		gt_free(fifo);
		return NULL;
	}
	
	fifo->magic = GT_MAGIC_FIFO;

	return fifo;
}

int GtFifo_Free(GtFifo_t *fifo) {
	if (fifo == NULL) return GT_ERROR_NULL_POINTER;
	if (fifo->magic != GT_MAGIC_FIFO) return GT_ERROR_MAGIC;

	fifo->magic = GT_MAGIC_UNKNOWN;

	gt_free(fifo->data);
	
    GtLock_Free((GtLock_t *)fifo->lock);
    
    gt_free(fifo);
    
    return GT_OK;
}

int GtFifo_Lock(GtFifo_t *fifo) {
	if (fifo == NULL) return GT_ERROR_NULL_POINTER;
	if (fifo->magic != GT_MAGIC_FIFO) return GT_ERROR_MAGIC;

	if (fifo->lock == NULL) {
		fifo->lock = GtLock_New();
		if (fifo->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)fifo->lock);
}

int GtFifo_Unlock(GtFifo_t *fifo) {
	if (fifo == NULL) return GT_ERROR_NULL_POINTER;
	if (fifo->magic != GT_MAGIC_FIFO) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)fifo->lock);
}

int GtFifo_Test(GtFifo_t *fifo) {
	if (fifo == NULL) return GT_ERROR_NULL_POINTER;
	if (fifo->magic != GT_MAGIC_FIFO) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtFifo_Init(GtFifo_t *gtFifo, gt_utf8 *elementType, int elementSize, int reservedLength)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	if(elementSize <= 0) return GT_ERROR_PARAMETER_2;
	if(reservedLength <= 0) return GT_ERROR_PARAMETER_3;

	gtFifo->fr = gtFifo->bk = gtFifo->length = 0;
	gtFifo->elementSize = elementSize;
	gtFifo->reservedLength = reservedLength;
	gtFifo->data = gt_calloc(reservedLength, elementSize);
	strcpy((char*)gtFifo->elementType, (char*)elementType);

	return GT_OK;
}

gt_utf8 *GtFifo_GetElementType(GtFifo_t *gtFifo)
{
	return gtFifo->elementType;
}

int GtFifo_GetElementSize(GtFifo_t *gtFifo)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	return gtFifo->elementSize;
}

int GtFifo_Push(GtFifo_t *gtFifo, void *element)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	if(gtFifo->length >= gtFifo->reservedLength)
	{
		//size not enough
		return -1;
	}

	gt_memcpy((gt_byte *)gtFifo->data + gtFifo->elementSize * gtFifo->bk, element, gtFifo->elementSize);
	gtFifo->bk++;
	gtFifo->bk %= gtFifo->reservedLength;
	gtFifo->length++;
	return GT_OK;
}

int GtFifo_Pop(GtFifo_t *gtFifo, void *element)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	if(gtFifo->length == 0)
	{
		//pop nothing
		return -1;
	}

	gt_memcpy(element, (gt_byte *)gtFifo->data + gtFifo->elementSize * gtFifo->fr, gtFifo->elementSize);
	gtFifo->fr++;//%=
	gtFifo->fr %= gtFifo->reservedLength;
	gtFifo->length--;
	return GT_OK;
}

int GtFifo_Empty(GtFifo_t *gtFifo)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	return gtFifo->length == 0;
}

int GtFifo_GetLength(GtFifo_t *gtFifo)
{
	if(gtFifo == NULL) return GT_ERROR_NULL_POINTER;
	return gtFifo->length;
}

#ifdef __cplusplus
}
#endif
