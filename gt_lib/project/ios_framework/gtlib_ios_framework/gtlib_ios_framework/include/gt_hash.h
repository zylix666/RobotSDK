/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_HASH_H__
#define __GT_HASH_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

int GtMd5_GetString(char *str, int strSize, void *data, int dataLength);

int GtSha1_GetString(char *str, int strSize, void *data, int dataLength);

int GtSha256_GetString(char *str, int strSize, void *data, int dataLength);

#ifdef __cplusplus
}
#endif

#endif /* __GT_HASH_H__ */
