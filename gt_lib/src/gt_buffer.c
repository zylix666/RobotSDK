/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

static int gtBufferIsInit = 0;

int GtBuffer_InitLib() {
	if (gtBufferIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtBufferIsInit = 1;
	
	return GT_OK;
}

GtBuffer_t *GtBuffer_New(int size, int align) {
	
	    int ret;
		GtBuffer_t *buffer;
	if (gtBufferIsInit == 0) return NULL;
	if (size < 0) return NULL;
	if (align < 0) return NULL;


    
    buffer = gt_calloc(1, sizeof(GtBuffer_t));
    if (buffer == NULL) {
    	return NULL;
    }
    
    buffer->align = align;
    
    ret = GtBuffer_SetSize(buffer, size);
    if (ret <= 0) {
    	gt_free(buffer);
        return NULL;
    }

    buffer->magic = GT_MAGIC_BUFFER;
    
    return buffer;
}

int GtBuffer_Free(GtBuffer_t *buffer) {
	if (buffer == NULL) return GT_ERROR_NULL_POINTER;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

	buffer->magic = GT_MAGIC_UNKNOWN;

	gt_free(buffer->buf);
	
	GtLock_Free(buffer->lock);
    
    gt_free(buffer);
    
    return GT_OK;
}

int GtBuffer_Lock(GtBuffer_t *buffer) {
	if (buffer == NULL) return GT_ERROR_NULL_POINTER;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

	if (buffer->lock == NULL) {
		buffer->lock = GtLock_New();
		if (buffer->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(buffer->lock);
}

int GtBuffer_Unlock(GtBuffer_t *buffer) {
	if (buffer == NULL) return GT_ERROR_NULL_POINTER;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

	return GtLock_Unlock(buffer->lock);
}

int GtBuffer_SetSize(GtBuffer_t *buffer, int size) {
	int rest;
	gt_byte *bufTemp = NULL;
	if (buffer == NULL) return GT_ERROR_PARAMETER_0;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_PARAMETER_0;
	if (size <= 0) return GT_ERROR_PARAMETER_1;

    if (buffer->size == size) {
		return GT_OK;
	}
    
	rest = size % buffer->align;
	size += rest;
    
	
    
	bufTemp = (gt_byte *) gt_malloc(size);
	if (bufTemp == NULL) {
		return GT_ERROR_NO_MEMORY;
    }
   
	if (buffer->size > 0) {
		if (buffer->size >= size) {
			gt_memcpy(bufTemp, buffer->buf, size);
            buffer->length = size;
		} else {
			gt_memcpy(bufTemp, buffer->buf, buffer->size);
		}
	}
    
	if (buffer->buf != NULL) {
		gt_free(buffer->buf);
	}
    
	buffer->size = size;
	buffer->buf = bufTemp;
    
    return GT_OK;
}

int GtBuffer_Append(GtBuffer_t *buffer, gt_byte *buf, int length) {
	int ret;
	int length_new = buffer->length + length;
	if (buffer == NULL) return GT_ERROR_PARAMETER_0;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_PARAMETER_0;
	if (buf == NULL) return GT_ERROR_PARAMETER_1;
	if (length < 0) return GT_ERROR_PARAMETER_2;

	

	if (length_new > buffer->size) {
		ret = GtBuffer_SetSize(buffer, length_new);
		if (ret <= 0) {
			return ret;
		}
	}
    
	gt_memcpy(buffer->buf + buffer->length, buf, length);
	buffer->length += length;
    
    return GT_OK;
}

int GtBuffer_GetLength(GtBuffer_t *buffer) {
    if (buffer == NULL) return GT_ERROR_NULL_POINTER;
    if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

    return buffer->length;
}

int GtBuffer_GetSize(GtBuffer_t *buffer) {
    if (buffer == NULL) return GT_ERROR_NULL_POINTER;
    if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

    return buffer->size;
}

int GtBuffer_Get(GtBuffer_t *buffer, gt_byte *buf, int length) {
	if (buffer == NULL) return GT_ERROR_PARAMETER_0;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_PARAMETER_0;
	if (buf == NULL) return GT_ERROR_PARAMETER_1;
	if (length <= 0) return GT_ERROR_PARAMETER_2;

	if (length > buffer->length) {
        length = buffer->length;
	}
    
	gt_memcpy(buf, buffer->buf, length);
    
	buffer->length -= length;
    
    if (buffer->length > 0) {
        gt_memmove(buffer->buf, buffer->buf + length, buffer->length);
    }
    
    return length;
}

int GtBuffer_Empty(GtBuffer_t *buffer) {
	if (buffer == NULL) return GT_ERROR_NULL_POINTER;
	if (buffer->magic != GT_MAGIC_BUFFER) return GT_ERROR_MAGIC;

    buffer->length = 0;
    
    return GT_OK;
}

#ifdef __cplusplus
}
#endif
