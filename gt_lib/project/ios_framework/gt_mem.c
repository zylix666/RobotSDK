/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_mem.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtMemIsInit  = 0;

static volatile int gtMemMallocGood  = 0;
static volatile int gtMemMallocBad   = 0;
static volatile int gtMemMallocZero  = 0;

static volatile int gtMemCallocGood  = 0;
static volatile int gtMemCallocBad   = 0;
static volatile int gtMemCallocZero  = 0;

static volatile int gtMemReallocGood = 0;
static volatile int gtMemReallocBad  = 0;
static volatile int gtMemReallocZero = 0;

static volatile int gtMemFreeGood = 0;
static volatile int gtMemFreeNull = 0;

int GtMem_InitLib() {
	if (gtMemIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtMemIsInit = 1;
	
    return GT_OK;
}

void *gt_malloc(int size) {
	if (size < 0) {
		gtMemMallocBad ++;
		return NULL;
	}

	if (size == 0) {
		gtMemMallocZero ++;
		return NULL;
	}

	void *ptr = malloc(size);
	if (ptr == NULL) {
		gtMemMallocBad ++;
		return NULL;
	}

	gtMemMallocGood ++;

	return ptr;
}

void *gt_calloc(int num, int size) {
	if (num < 0) {
		gtMemCallocBad ++;
		return NULL;
	}

	if (size < 0) {
		gtMemCallocBad ++;
		return NULL;
	}

	if (num == 0) {
		gtMemCallocZero ++;
		return NULL;
	}

	if (size == 0) {
		gtMemCallocZero ++;
		return NULL;
	}

	void *ptr = calloc(num, size);
	if (ptr == NULL) {
		gtMemCallocBad ++;
		return NULL;
	}

	gtMemCallocGood ++;

	return ptr;
}

void *gt_realloc(void *ptr, int size) {
	if (size < 0) {
		gtMemReallocBad ++;
		if (ptr != NULL) {
			free(ptr);	
		}
		return NULL;
	}

	if (size == 0) {
		gtMemReallocZero ++;
		if (ptr != NULL) {
			free(ptr);	
		}
		return NULL;
	}

	void *ptrNew = realloc(ptr, size);
	if (ptrNew == NULL) {
		gtMemReallocBad ++;
		return NULL;
	}

	gtMemReallocGood ++;

	return ptrNew;
}

int gt_free(void *ptr) {
	if (ptr == NULL) {
		gtMemFreeNull ++;
		return GT_OK;
	}

	free (ptr);

	gtMemFreeGood ++;

	return GT_OK;
}

int GtMem_PrintInfo() {
	GT_LOGI("Malloc Count", gtMemMallocGood + gtMemMallocBad + gtMemMallocZero);
	GT_LOGI("Malloc Good", gtMemMallocGood);
	GT_LOGI("Malloc Bad", gtMemMallocBad);
	GT_LOGI("Malloc Zero", gtMemMallocZero);

	GT_LOGI("Calloc Count", gtMemCallocGood + gtMemCallocBad + gtMemCallocZero);
	GT_LOGI("Calloc Good", gtMemCallocGood);
	GT_LOGI("Calloc Bad", gtMemCallocBad);
	GT_LOGI("Calloc Zero", gtMemCallocZero);

	GT_LOGI("Realloc Count", gtMemReallocGood + gtMemReallocBad + gtMemReallocZero);
	GT_LOGI("Realloc Good", gtMemReallocGood);
	GT_LOGI("Realloc Bad", gtMemReallocBad);
	GT_LOGI("Realloc Zero", gtMemReallocZero);

	GT_LOGI("Free Count", gtMemFreeGood + gtMemFreeNull);
	GT_LOGI("Free Good", gtMemFreeGood);
	GT_LOGI("Free NULL", gtMemFreeNull);

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
