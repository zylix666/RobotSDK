/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_SOUND_H__
#define __GT_SOUND_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    void        *data;
    int					 dataLength;
    int 				 bit;
    int					 channel;
    int          hz;
    void        *lock;
    GT_MAGIC_t   magic;
} GtSound_t;

int GtSound_InitLib();

GtSound_t *GtSound_New();

int GtSound_Free(GtSound_t *sound);

int GtSound_Lock(GtSound_t *sound);

int GtSound_Unlock(GtSound_t *sound);

int GtSound_LoadFromFile(GtSound_t *sound, gt_utf8 *path);

int GtSound_GetMainTone(GtSound_t *sound);

#ifdef __cplusplus
}
#endif

#endif /* __GT_SOUND_H__ */
