/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_MAGIC_H__
#define __GT_MAGIC_H__

#include "gt_lib.h"

typedef enum {
	GT_MAGIC_UNKNOWN       = 0,
	GT_MAGIC_IMAGE         = 0x5AA50001,
	GT_MAGIC_BUFFER        = 0x5AA50002,
	GT_MAGIC_LOCK          = 0x5AA50003,
	GT_MAGIC_SEMAPHORE     = 0x5AA50004,
	GT_MAGIC_TEMPLATE      = 0x5AA50005,
	GT_MAGIC_BITMAP        = 0x5AA50006,
	GT_MAGIC_COLOR         = 0x5AA50007,
	GT_MAGIC_FILE          = 0x5AA50008,
	GT_MAGIC_PDF           = 0x5AA50009,
	GT_MAGIC_HTTP          = 0x5AA5000A,
	GT_MAGIC_HTTPD         = 0x5AA5000B,
	GT_MAGIC_HASH_MAP      = 0x5AA5000C,
	GT_MAGIC_TIMER_US      = 0x5AA5000D,
	GT_MAGIC_SVM           = 0x5AA5000E,
	GT_MAGIC_MATRIX        = 0x5AA5000F,
	GT_MAGIC_ARRAY         = 0x5AA50010,
	GT_MAGIC_FIFO          = 0x5AA50011,
	GT_MAGIC_STACK         = 0x5AA50012,
	GT_MAGIC_IMAGE_OCV     = 0x5AA50013,
	GT_MAGIC_PID           = 0x5AA50014,

	GT_MAGIC_COUNT
} GT_MAGIC_t;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GT_MAGIC_H__ */
