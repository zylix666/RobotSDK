/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_HTTPD_H__
#define __GT_HTTPD_H__

#include "gt_lib.h"

typedef int (*GtHttpdCallback_t)(void *httpdPtr, void *requestPtr, void *responsePtr);

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
	int status;

	//GtBuffer_t *bufferRequest;

	//GtBuffer_t *bufferResponse;
    
	void        *lock;
	GT_MAGIC_t   magic;
} GtHttpdSession_t;

typedef struct {
	int toStop;
	int status;
	int port;
	int sessionsNum;
	int backlog;

	//GtArray_t *sessions;
	//GtFifo_t  *sessions_ready;
    
	void        *lock;
	GT_MAGIC_t   magic;
} GtHttpd_t;

int GtHttpd_InitLib(gt_utf8 *license, gt_utf8 *token);

GtHttpd_t *GtHttpd_New();

int GtHttpd_Free(GtHttpd_t *httpd);

int GtHttpd_Lock(GtHttpd_t *httpd);

int GtHttpd_Unlock(GtHttpd_t *httpd);

int GtHttpd_Start(GtHttpd_t *httpd);

int GtHttpd_Stop(GtHttpd_t *httpd);

#ifdef __cplusplus
}
#endif

#endif /* __GT_HTTPD_H__ */
