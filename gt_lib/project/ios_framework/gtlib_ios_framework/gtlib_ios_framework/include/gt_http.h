/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_HTTP_H__
#define __GT_HTTP_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {   
	void       *lock;
	GT_MAGIC_t  magic;
} GtHttp_t;

GtHttp_t *GtHttp_New();

int GtHttp_Free(GtHttp_t *http);

int GtHttp_Lock(GtHttp_t *http);

int GtHttp_Unlock(GtHttp_t *http);

int GtHttp_Get(GtHttp_t *http, gt_utf8 *url);

#ifdef __cplusplus
}
#endif

#endif /* __GT_HTTP_H__ */
