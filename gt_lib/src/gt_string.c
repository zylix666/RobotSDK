/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_string.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtStringIsInit  = 0;

int GtString_InitLib() {
	if (gtStringIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtStringIsInit = 1;
	
    return GT_OK;
}

void *gt_memset(void *ptr, int value, int num) {
	if (ptr == NULL) {
		return NULL;
	}

	if (value < 0) {
		return NULL;
	}

	if (num < 0) {
		return NULL;
	}

	return memset(ptr, value, num);
}

void *gt_memcpy(void *destination, const void *source, int num) {
	if (destination == NULL) {
		return NULL;
	}

	if (source == NULL) {
		return NULL;
	}

	if (num < 0) {
		return NULL;
	}

	return memcpy(destination, source, num);
}

void *gt_memmove(void *destination, const void *source, int num) {
	if (destination == NULL) {
		return NULL;
	}

	if (source == NULL) {
		return NULL;
	}

	if (num < 0) {
		return NULL;
	}

	return memmove(destination, source, num);
}

int gt_memcmp(const void *ptr1, const void *ptr2, int num) {
	if (ptr1 == NULL) {
		return GT_ERROR_PARAMETER_0;
	}

	if (ptr2 == NULL) {
		return GT_ERROR_PARAMETER_1;
	}

	if (num < 0) {
		return GT_ERROR_PARAMETER_2;
	}

	return memcmp(ptr1, ptr2, num);
}

#ifdef __cplusplus
}
#endif
