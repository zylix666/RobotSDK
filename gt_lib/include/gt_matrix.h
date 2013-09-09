/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_MATRIX_H__
#define __GT_MATRIX_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    int n;
    int m;
    double      **value;

    void        *lock;
    GT_MAGIC_t   magic;
} GtMatrix_t;

GtMatrix_t *GtMatrix_New();

int GtMatrix_Free(GtMatrix_t *matrix);

int GtMatrix_Lock(GtMatrix_t *matrix);

int GtMatrix_Unlock(GtMatrix_t *matrix);

int GtMatrix_Test(GtMatrix_t *matrix);

int GtMatrix_Init(GtMatrix_t *matrix, int n, int m);

int GtMatrix_Set(GtMatrix_t *matrix, int n, int m, double value);

int GtMatrix_Get(GtMatrix_t *matrix, int x, int y, double *value);

int GtMatrix_Add(GtMatrix_t *matrix_a, GtMatrix_t *matrix_b, GtMatrix_t *matrix_ab);

int GtMatrix_Mul(GtMatrix_t *matrix_a, GtMatrix_t *matrix_b, GtMatrix_t *matrix_ab);

#ifdef __cplusplus
}
#endif

#endif /* __GT_MATRIX_H__ */
