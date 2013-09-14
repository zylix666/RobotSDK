/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_http.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtHttpIsInit = 0;

int GtHttp_InitLib() {
	if (gtHttpIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtHttpIsInit = 1;

	return GT_OK;
}

GtHttp_t *GtHttp_New() {
	if (gtHttpIsInit == 0) return NULL;
	
	GtHttp_t *http = (GtHttp_t *)gt_calloc(1, sizeof(GtHttp_t));
	if (http == NULL) {
		return NULL;
	}
	
	http->magic = GT_MAGIC_HTTP;

	return http;
}

int GtHttp_Free(GtHttp_t *http) {
	if (http == NULL) return GT_ERROR_NULL_POINTER;
	if (http->magic != GT_MAGIC_HTTP) return GT_ERROR_MAGIC;

	http->magic = GT_MAGIC_UNKNOWN;
	
    GtLock_Free(http->lock);
    
    gt_free(http);
    
    return GT_OK;
}

int GtHttp_Lock(GtHttp_t *http) {
	if (http == NULL) return GT_ERROR_NULL_POINTER;
	if (http->magic != GT_MAGIC_HTTP) return GT_ERROR_MAGIC;

	if (http->lock == NULL) {
		http->lock = GtLock_New();
		if (http->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(http->lock);
}

int GtHttp_Unlock(GtHttp_t *http) {
	if (http == NULL) return GT_ERROR_NULL_POINTER;
	if (http->magic != GT_MAGIC_HTTP) return GT_ERROR_MAGIC;

	return GtLock_Unlock(http->lock);
}

int GtHttp_Get(GtHttp_t *http, gt_utf8 *url) {
	if (http == NULL) return GT_ERROR_PARAMETER_0;
	if (http->magic != GT_MAGIC_HTTP) return GT_ERROR_PARAMETER_0;
	if (url == NULL) return GT_ERROR_PARAMETER_1;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
