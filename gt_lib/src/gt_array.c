/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_array.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtArrayIsInit = 0;

int GtArray_InitLib() {
	if (gtArrayIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtArrayIsInit = 1;

	return GT_OK;
}

GtArray_t *GtArray_New() {

	GtArray_t *gtArray;
	if (gtArrayIsInit == 0) return NULL;
	
	gtArray = (GtArray_t *)gt_calloc(1, sizeof(GtArray_t));
	if (gtArray == NULL) {
		return NULL;
	}
	
	gtArray->magic = GT_MAGIC_ARRAY;

	return gtArray;
}

int GtArray_Free(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	gtArray->magic = GT_MAGIC_UNKNOWN;

	gt_free(gtArray->data);
	
    GtLock_Free((GtLock_t *)gtArray->lock);
    
    gt_free(gtArray);
    
    return GT_OK;
}

int GtArray_Lock(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	if (gtArray->lock == NULL) {
		gtArray->lock = GtLock_New();
		if (gtArray->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock((GtLock_t *)gtArray->lock);
}

int GtArray_Unlock(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return GtLock_Unlock((GtLock_t *)gtArray->lock);
}

int GtArray_Test(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtArray_Init(GtArray_t *gtArray, gt_utf8 *elementType, int elementSize, int reservedLength) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if(elementSize <= 0) return GT_ERROR_PARAMETER_2;
	if(reservedLength <= 0) return GT_ERROR_PARAMETER_3;

	if (elementType != NULL) {
		strcpy((char*)gtArray->elementType, (char*)elementType);
	}
	gtArray->length = 0;
	gtArray->elementSize = elementSize;
	gtArray->realLength = reservedLength;
	gtArray->reservedLength = reservedLength;
	gtArray->data = gt_calloc(reservedLength, elementSize);

	return GT_OK;
}

gt_utf8 *GtArray_GetElementType(GtArray_t *gtArray) {
	if (gtArray == NULL) return NULL;
	if (gtArray->magic != GT_MAGIC_ARRAY) return NULL;

	return gtArray->elementType;
}

int GtArray_GetElementSize(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return gtArray->elementSize;
}

void *GtArray_Get(GtArray_t *gtArray, int index) {
	if (gtArray == NULL) return NULL;
	if (gtArray->magic != GT_MAGIC_ARRAY) return NULL;
	if (index < 0) return NULL;
	if (index >= gtArray->length) return NULL;

	return (unsigned char *)gtArray->data + index * gtArray->elementSize;
}

int GtArray_Copy(GtArray_t *gtArray, int index, void *element) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (index < 0) return GT_ERROR_PARAMETER_1;
	if (index >= gtArray->length) return GT_ERROR_PARAMETER_1;
	if (element == NULL) return GT_ERROR_PARAMETER_2;

	gt_memcpy(element, (unsigned char *)gtArray->data + index * gtArray->elementSize, gtArray->elementSize);

	return GT_OK;
}

int GtArray_Set(GtArray_t *gtArray, int index, void *element) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (index < 0) return GT_ERROR_PARAMETER_1;
	if (index >= gtArray->length) return GT_ERROR_PARAMETER_1;
	if (element == NULL) return GT_ERROR_PARAMETER_2;

	gt_memcpy((unsigned char *)gtArray->data + index * gtArray->elementSize, element, gtArray->elementSize);

	return GT_OK;
}

int GtArray_Append(GtArray_t *gtArray, void *element) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (element == NULL) return GT_ERROR_PARAMETER_1;

	if(gtArray->realLength < gtArray->length + 1) {
		GtArray_SetRealLength(gtArray, gtArray->realLength + gtArray->reservedLength + 1);
	}

	gt_memcpy((unsigned char *)gtArray->data + gtArray->length * gtArray->elementSize, element, gtArray->elementSize);
	gtArray->length ++;

	return GT_OK;
}

int GtArray_Insert(GtArray_t *gtArray, int index, void *element) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (index < 0) return GT_ERROR_PARAMETER_1;
	if (index >= gtArray->length) return GT_ERROR_PARAMETER_1;
	if (element == NULL) return GT_ERROR_PARAMETER_2;
	
	if(gtArray->realLength < gtArray->length + 1) {
		GtArray_SetRealLength(gtArray, gtArray->realLength + gtArray->reservedLength + 1);
	}

	if (index < gtArray->length - 1) {
		gt_memmove((unsigned char *)gtArray->data + (index + 1) * gtArray->elementSize, (unsigned char *)gtArray->data + index * gtArray->elementSize, gtArray->length * gtArray->elementSize);
	}

	/*
	int i;
	for (i = gtArray->length - 1; i >= index; i--) {
		gt_memcpy(gtArray->data + (i+1) * gtArray->elementSize, gtArray->data + i * gtArray->elementSize, gtArray->elementSize);
	}
	*/
	gt_memcpy((unsigned char *)gtArray->data + index * gtArray->elementSize, element, gtArray->elementSize);
	
	gtArray->length++;
	
	return GT_OK;
}

int GtArray_Remove(GtArray_t *gtArray, int index) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (gtArray->length <= 0) return GT_ERROR_PARAMETER_0;
	if (index < 0) return GT_ERROR_PARAMETER_1;
	if (index >= gtArray->length) return GT_ERROR_PARAMETER_1;

	if (index < gtArray->length - 1) {
		gt_memmove((unsigned char *)gtArray->data + index * gtArray->elementSize, (unsigned char *)gtArray->data + (index + 1) * gtArray->elementSize, (gtArray->length - 1) * gtArray->elementSize);
	}


	/*
	int i;
	for(i=index+1; i<gtArray->length; i++)
		gt_memcpy(gtArray->data + (i-1) * gtArray->elementSize, gtArray->data + i * gtArray->elementSize, gtArray->elementSize);
	*/

	gtArray->length--;

	return GT_OK;
}

int GtArray_SetReservedLength(GtArray_t *gtArray, int reservedLength) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (reservedLength <= 0) return GT_ERROR_PARAMETER_1;

	gtArray->reservedLength = reservedLength;
	if(gtArray->realLength - gtArray->length < gtArray->reservedLength) {
		GtArray_SetRealLength(gtArray, gtArray->realLength + gtArray->reservedLength);
	}

	return GT_OK;
}

int GtArray_SetRealLength(GtArray_t *gtArray, int realLength) {
	
	int length;

	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (realLength <= 0) return GT_ERROR_PARAMETER_1;


	if(realLength - gtArray->length > gtArray->reservedLength) {
		length = realLength;
	} else {
		length = realLength + gtArray->reservedLength;
	}

	if(length > gtArray->realLength) {
		void *data = gt_calloc(length, gtArray->elementSize);
		gt_memcpy(data, gtArray->data, gtArray->realLength * gtArray->elementSize);
		gt_free(gtArray->data);
		gtArray->data = data;
		gtArray->realLength = length;
	}
	else if(length < gtArray->realLength) {
		void *data = gt_calloc(length, gtArray->elementSize);
		gt_memcpy(data, gtArray->data, length * gtArray->elementSize);
		gt_free(gtArray->data);
		gtArray->data = data;
		gtArray->realLength = length;
		if(gtArray->length > gtArray->realLength) {
			gtArray->length = gtArray->realLength;
		}
	}
	return GT_OK;
}

int GtArray_SetLength(GtArray_t *gtArray, int length) {
	if (gtArray == NULL) return GT_ERROR_PARAMETER_0;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_PARAMETER_0;
	if (length <= 0) return GT_ERROR_PARAMETER_1;

	if (length + gtArray->reservedLength > gtArray->realLength) {
		GtArray_SetRealLength(gtArray, length + gtArray->reservedLength);
	}

	gtArray->length = length;

	return GT_OK;
}

int GtArray_GetRealLength(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return gtArray->realLength;
}

int GtArray_GetReservedLength(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return gtArray->reservedLength;
}

int GtArray_GetLength(GtArray_t *gtArray) {
	if (gtArray == NULL) return GT_ERROR_NULL_POINTER;
	if (gtArray->magic != GT_MAGIC_ARRAY) return GT_ERROR_MAGIC;

	return gtArray->length;
}

int GtArray_CheckElementType(GtArray_t *gtArray, gt_utf8 *elementType)
{
	return !strcmp((char*)gtArray->elementType, (char*)elementType);
}

#ifdef __cplusplus
}
#endif
