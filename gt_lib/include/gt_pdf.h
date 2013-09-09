/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_PDF_H__
#define __GT_PDF_H__

#include "gt_lib.h"

typedef struct {
	void       *lock;
	GT_MAGIC_t  magic;
} GtPdf_t;

#ifdef __cplusplus
extern "C"
{
#endif

int GtPdf_InitLib(gt_utf8 *license, gt_utf8 *token);

GtPdf_t *GtPdf_New();

int GtPdf_Free(GtPdf_t *gtPdf);

int GtPdf_Lock(GtPdf_t *gtPdf);

int GtPdf_Unlock(GtPdf_t *gtPdf);

int GtPdf_OpenFile(GtPdf_t *gtPdf, gt_utf8 *path , int flags);

int GtPdf_Close(GtPdf_t *gtPdf);

#ifdef __cplusplus
}
#endif
   
#endif /* __GT_PDF_H__ */
