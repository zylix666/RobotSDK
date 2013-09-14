/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_CONFIG_H__
#define __GT_CONFIG_H__

#include <stdio.h>
#include <wchar.h>
#include <gt_lib.h>
#include <gt_utf8.h>
#include <gt_type.h>

#ifdef __cplusplus
extern "C" 
{
#endif

int GtConfigParser_Open(gt_utf8 *path);

int GtConfigParser_Get(gt_utf8 *key, gt_utf8 *value, int valueSize);

int GtConfigParser_Close();

#ifdef __cplusplus
}
#endif

#endif /* __GT_CONFIG_H__ */


