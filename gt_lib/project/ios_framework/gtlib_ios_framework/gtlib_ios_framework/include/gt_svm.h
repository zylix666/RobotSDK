/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_SVM_H__
#define __GT_SVM_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    void        *model;
    void        *problem;
    void        *parameter;
    
    void        *lock;
    GT_MAGIC_t   magic;
} GtSvm_t;

int GtSvm_InitLib();

GtSvm_t *GtSvm_New();

int GtSvm_Free(GtSvm_t *svm);

int GtSvm_Lock(GtSvm_t *svm);

int GtSvm_Unlock(GtSvm_t *svm);

int GtSvm_Test(GtSvm_t *svm);

int GtSvm_LoadCsv(GtSvm_t *svm, gt_utf8 *path, int column, int row);

int GtSvm_DoCrossValidation(GtSvm_t *svm, int fold);

int GtSvm_Train(GtSvm_t *svm, gt_utf8 *modelPath);

int GtSvm_Predict(GtSvm_t *svm, int num, double *value);

int GtSvm_LoadModel(GtSvm_t *svm, gt_utf8 *modelPath);

#ifdef __cplusplus
}
#endif

#endif /* __GT_SVM_H__ */
