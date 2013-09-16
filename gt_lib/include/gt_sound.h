/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_SOUND_H__
#define __GT_SOUND_H__
typedef double SOUND_T;
#include "gt_lib.h"
#include "sndfile.h"
#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct {
    SOUND_T	*data;
    SOUND_T     *fft;
    int		dataLength;
    int 	bit;
    int		channel;
    int		hz;
    void	*lock;
    int 	Nfft;
    GT_MAGIC_t	magic;
} GtSound_t;

int GtSound_InitLib();

GtSound_t *GtSound_New();

int GtSound_Free(GtSound_t *sound);

int GtSound_Lock(GtSound_t *sound);
/******************** internal FFt function *************************/
void FFT(double x[] , double y[] , int n , int sign);
/********************************************************************/

char * GtSound_GetInfo(GtSound_t * sound);

int GtSound_Unlock(GtSound_t *sound);

int GtSound_LoadFromFile(GtSound_t *sound, gt_utf8 *path);

double GtSound_GetMainTone(GtSound_t *sound);

#ifdef __cplusplus
}
#endif

#endif /* __GT_SOUND_H__ */
