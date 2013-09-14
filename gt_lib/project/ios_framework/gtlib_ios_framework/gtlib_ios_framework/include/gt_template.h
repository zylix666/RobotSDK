/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_TEMPLATE_H__
#define __GT_TEMPLATE_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    void        *data;
    void        *lock;
    GT_MAGIC_t   magic;
} GtTemplate_t;

int GtTemplate_InitLib();

GtTemplate_t *GtTemplate_New();

int GtTemplate_Free(GtTemplate_t *template);

int GtTemplate_Lock(GtTemplate_t *template);

int GtTemplate_Unlock(GtTemplate_t *template);

int GtTemplate_Test(GtTemplate_t *template);

#ifdef __cplusplus
}
#endif

#endif /* __GT_TEMPLATE_H__ */
