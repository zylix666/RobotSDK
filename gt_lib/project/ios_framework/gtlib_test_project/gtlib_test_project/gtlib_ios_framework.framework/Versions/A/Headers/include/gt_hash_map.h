/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_HASH_MAP_H__
#define __GT_HASH_MAP_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct GtHashMapData_t {
	void *value;
	int   valueSize;

	void *key;
	int   keySize;
	int   hash;
	
	struct GtHashMapData_t *next;
} GtHashMapData_t;

typedef struct {
	GtHashMapData_t ** data;
	int size;
	int length;

	void *lock;
	GT_MAGIC_t magic;
} GtHashMap_t;

int GtHashMap_InitLib();
    
GtHashMap_t *GtHashMap_New();

int GtHashMap_Free(GtHashMap_t *hashMap);

int GtHashMap_Lock(GtHashMap_t *hashMap);

int GtHashMap_Unlock(GtHashMap_t *hashMap);

int GtHashMap_Append(GtHashMap_t *hashMap, void *key, int keyLength, void *value, int valueLength);

int GtHashMap_GetLength(GtHashMap_t *hashMap);

int GtHashMap_GetSize(GtHashMap_t *hashMap);

int GtHashMap_Get(GtHashMap_t *hashMap, GT_IN void *key, int keyLength, void *value, int valueSize);

#ifdef __cplusplus
}
#endif

#endif /* __GT_HASH_MAP_H__ */
