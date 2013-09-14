/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_hash_map.h"

#define GT_HASH_MAP_INIT_SIZE 16777216 // power of 2
#define GT_HASH_MAP_MUTIPLE 10
#define GT_HASH_MAP_INCEASE 2

#ifdef __cplusplus
extern "C"
{
#endif

static int gtHashMapIsInit = 0;

static int gtHashMapSsum = 0;

int GtHashMap_InitLib() {
	if (gtHashMapIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtHashMapIsInit = 1;

	return GT_OK;
}

GtHashMap_t *GtHashMap_New() {
	if (gtHashMapIsInit == 0) return NULL;

	GtHashMap_t *hashMap = gt_calloc(1, sizeof(GtHashMap_t));
	if (hashMap == NULL) {
		return NULL;
	}

	hashMap->data = (GtHashMapData_t **)gt_calloc(GT_HASH_MAP_INIT_SIZE  ,sizeof(GtHashMapData_t *));
	if (hashMap->data == NULL) {
		return NULL;
	}

	hashMap->size= GT_HASH_MAP_INIT_SIZE;
	hashMap->magic = GT_MAGIC_HASH_MAP;

	return hashMap;
}

int GtHashMap_Free(GtHashMap_t *hashMap) {
	if (hashMap == NULL) return GT_ERROR_NULL_POINTER;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_MAGIC;

	GtHashMapData_t *cur, *next;
	int i;
	for (i = 0; i < hashMap->size; i++) {
		cur = hashMap->data[i];
		while (cur != NULL) {
			next = cur->next;
			gt_free(cur->value);
			gt_free(cur->key);
			gt_free(cur);
			cur = next;
		}
	}

	gt_free(hashMap->data);

	hashMap->magic = GT_MAGIC_UNKNOWN;
	GtLock_Free(hashMap->lock); 
	gt_free(hashMap);

	return GT_OK;
}

int GtHashMap_Lock(GtHashMap_t *hashMap) {
	if (hashMap == NULL) return GT_ERROR_NULL_POINTER;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_MAGIC;

	if (hashMap->lock == NULL) {
		hashMap->lock = GtLock_New();
		if (hashMap->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(hashMap->lock);
}

int GtHashMap_Unlock(GtHashMap_t *hashMap) {
	if (hashMap == NULL) return GT_ERROR_NULL_POINTER;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_MAGIC;

	return GtLock_Unlock(hashMap->lock);
}

unsigned int gtHashMap_BKDR(char *str, int len) {
	unsigned int ret = 0;
	unsigned int seed = 131;

	int i;
	for (i = 0; i < len; i++) {
		ret = ret * seed + (*str);
		str++;
	}
	return ret & 0x7FFFFFFF;
}

int gtHashMap_CompareData(GtHashMapData_t *data, void *key, int keyLength) {
	gt_byte *cmp1 = (gt_byte *)data->key;
	gt_byte *cmp2 = (gt_byte *)key;

	if (data->keySize != keyLength)
		return 0;

	int i;
	for (i = 0; i < keyLength; i++) {
		if (cmp1[i] != cmp2[i]) {
			return 0;
		}
	}

	return 1;
}

int GtHashMap_Append(GtHashMap_t *hashMap, void *key, int keyLength, void *value, int valueLength) {
	if (hashMap == NULL) return GT_ERROR_PARAMETER_0;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_PARAMETER_0;
	if (key == NULL) return GT_ERROR_PARAMETER_1;
	if (keyLength <= 0) return GT_ERROR_PARAMETER_2;
	if (value == NULL) return GT_ERROR_PARAMETER_3;
	if (valueLength <= 0) return GT_ERROR_PARAMETER_4;

	char *id = (char *)key;
	int bkdr = gtHashMap_BKDR(id, keyLength);

	// same key exists
	GtHashMapData_t *data = hashMap->data[bkdr  & (hashMap->size - 1)];
	while (data != NULL) {
		if (data->hash == bkdr  && data->keySize == keyLength && gtHashMap_CompareData(data, key, keyLength)) {
			gt_free(data->value);

			data->valueSize = valueLength;
			data->value = gt_malloc(valueLength);
			gt_memcpy(data->value, value, valueLength);
			return GT_OK;
		}
		data = data->next;
	}

	data = (GtHashMapData_t *)gt_malloc(sizeof(GtHashMapData_t));
	data->hash = bkdr;
	data->valueSize = valueLength;
	data->keySize = keyLength;
	data->next = NULL;

	data->key = gt_malloc(keyLength);
	gt_memcpy(data->key, key, keyLength);

	data->value = gt_malloc(valueLength);
	gt_memcpy(data->value, value, valueLength);

	// Increase hash map size.
	if ((hashMap->length + 1) * GT_HASH_MAP_MUTIPLE > hashMap->size) {
		GtHashMapData_t **dataOld = hashMap->data;
		GtHashMapData_t *cur, *next;

		int oldLength = hashMap->size;
		int newLength = hashMap->size * GT_HASH_MAP_INCEASE;
		
		hashMap->data = (GtHashMapData_t **)gt_calloc(newLength, sizeof(GtHashMapData_t *));
		
		int i;
		for (i = 0; i < oldLength; i++) {
			cur = dataOld[i];
			while (cur != NULL) {
				next = cur->next;

				cur->next = hashMap->data[cur->hash & (newLength - 1)];
				hashMap->data[cur->hash & (newLength - 1)] = cur;  // ?
				cur = next;
			}

			gt_free(dataOld[i]);  // ?
		}

		gt_free(dataOld);

		hashMap->size = newLength;
	}

	int pos = bkdr & (hashMap->size - 1);
	data->next = hashMap->data[pos];  // ?
	hashMap->data[pos] = data;
	hashMap->length++;

	return GT_OK;
}

int GtHashMap_GetSize(GtHashMap_t *hashMap) {
	if (hashMap == NULL) return GT_ERROR_NULL_POINTER;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_MAGIC;

	return hashMap->size;
}

int GtHashMap_GetLength(GtHashMap_t *hashMap) {
	if (hashMap == NULL) return GT_ERROR_NULL_POINTER;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_MAGIC;

	return hashMap->length;
}

int GtHashMap_Get(GtHashMap_t *hashMap, void *key, int keyLength, void *value, int valueSize) {
	if (hashMap == NULL) return GT_ERROR_PARAMETER_0;
	if (hashMap->magic != GT_MAGIC_HASH_MAP) return GT_ERROR_PARAMETER_0;
	if (key == NULL) return GT_ERROR_PARAMETER_1;
	if (keyLength <= 0) return GT_ERROR_PARAMETER_2;
	if (value == NULL) return GT_ERROR_PARAMETER_3;
	if (valueSize <= 0) return GT_ERROR_PARAMETER_4;

	char *id = (char *)key;
	int bkdr = gtHashMap_BKDR(id, keyLength);
	int pos = bkdr & (hashMap->size - 1);

	GtHashMapData_t *cur = hashMap->data[pos];
	while (cur != NULL) {
		gtHashMapSsum ++;
		if (cur->hash == bkdr && cur->keySize == keyLength && gtHashMap_CompareData(cur, key, keyLength)) {
			if (valueSize < cur->valueSize) {
				return GT_ERROR_PARAMETER_4;
			}

			gt_memcpy(value, cur->value, cur->valueSize);
			return cur->valueSize;
		}
		cur = cur->next;
	}

	return GT_ERROR_NO_MATCH;
}

void test(GtHashMap_t *hashMap) {
	fprintf(stderr,"gtHashMapSsum = %d / query = %d\n", gtHashMapSsum, 1000000);
	int mx=0, cnt;
	GtHashMapData_t *cur;

	int i;
	for (i = 0; i < hashMap->size; i++) {
		cur = hashMap->data[i];
		cnt = 0;
		while (cur != NULL) {
			cnt++;
			cur = cur->next;
		}

		if (mx < cnt) {
			mx = cnt;
			fprintf(stderr,"new mx = %d\n",mx);
		}
	}

}

#ifdef __cplusplus
}
#endif

