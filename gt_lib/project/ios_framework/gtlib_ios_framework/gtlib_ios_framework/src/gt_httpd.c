/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (httpd://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include <gt_httpd.h>

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtHttpdIsInit = 0;

int GtHttpd_InitLib(gt_utf8 *license, gt_utf8 *token) {
	if (gtHttpdIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtHttpdIsInit = 1;

	return GT_OK;
}

GtHttpd_t *GtHttpd_New() {
	if (gtHttpdIsInit == 0) return NULL;
	
	GtHttpd_t *httpd = (GtHttpd_t *)gt_calloc(1, sizeof(GtHttpd_t));
	if (httpd == NULL) {
		return NULL;
	}
	
	httpd->magic = GT_MAGIC_HTTPD;

	return httpd;
}

int GtHttpd_Free(GtHttpd_t *httpd) {
	if (httpd == NULL) return GT_ERROR_NULL_POINTER;
	if (httpd->magic != GT_MAGIC_HTTPD) return GT_ERROR_MAGIC;

	httpd->magic = GT_MAGIC_UNKNOWN;
	
    GtLock_Free(httpd->lock);
    
    gt_free(httpd);
    
    return GT_OK;
}

int GtHttpd_Lock(GtHttpd_t *httpd) {
	if (httpd == NULL) return GT_ERROR_NULL_POINTER;
	if (httpd->magic != GT_MAGIC_HTTPD) return GT_ERROR_MAGIC;

	if (httpd->lock == NULL) {
		httpd->lock = GtLock_New();
		if (httpd->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(httpd->lock);
}

int GtHttpd_Unlock(GtHttpd_t *httpd) {
	if (httpd == NULL) return GT_ERROR_NULL_POINTER;
	if (httpd->magic != GT_MAGIC_HTTPD) return GT_ERROR_MAGIC;

	return GtLock_Unlock(httpd->lock);
}

int GtHttpd_Start(GtHttpd_t *httpd) {
	if (httpd == NULL) return GT_ERROR_NULL_POINTER;
	if (httpd->magic != GT_MAGIC_HTTPD) return GT_ERROR_MAGIC;

	httpd->toStop = 0;

	return GT_OK;
}

int GtHttpd_Stop(GtHttpd_t *httpd) {
	if (httpd == NULL) return GT_ERROR_NULL_POINTER;
	if (httpd->magic != GT_MAGIC_HTTPD) return GT_ERROR_MAGIC;

	httpd->toStop = 1;

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
