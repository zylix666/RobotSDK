/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_MEDIA_PLAYER_H__
#define __GT_MEDIA_PLAYER_H__

#include "gt_lib.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum {
	GT_MEDIA_PLAYER_PLAY_AUDIO = 4,
	GT_MEDIA_PLAYER_PLAY_VIDEO = 3,
	GT_MEDIA_PLAYER_PLAY_VIDEO_AUDIO = 2,
	GT_MEDIA_PLAYER_PREPARE_PLAY = 1,
	GT_MEDIA_PLAYER_STOP = 0,
    GT_MEDIA_PLAYER_ERROR = -1,
	GT_MEDIA_PLAYER_ERROR_IS_PLAYING = -2,
	GT_MEDIA_PLAYER_ERROR_THREAD = -3,
	GT_MEDIA_PLAYER_ERROR_ALLOC_FORMAT = -4,
	GT_MEDIA_PLAYER_ERROR_OPEN = -5,
	GT_MEDIA_PLAYER_ERROR_FIND_STREAM_INFO = -6,
	GT_MEDIA_PLAYER_ERROR_FIND_VIDEO_DECODER = -7,
	GT_MEDIA_PLAYER_ERROR_OPEN_VIDEO_DECODER = -8,
	GT_MEDIA_PLAYER_ERROR_FIND_AUDIO_DECODER = -9,
	GT_MEDIA_PLAYER_ERROR_OPEN_AUDIO_DECODER = -10,
	GT_MEDIA_PLAYER_ERROR_GET_VIDEO_CONVERTER = -11,
	GT_MEDIA_PLAYER_ERROR_NO_VIDEO_AUDIO = -12,
    GT_MEDIA_PLAYER_ERROR_NULL_POINTER = -13,
    GT_MEDIA_PLAYER_ERROR_NO_VIDEO = -14,
    GT_MEDIA_PLAYER_ERROR_NO_AUDIO = -15
} GT_MEDIA_PLAYER_STATUS;

typedef enum {
	GT_MEDIA_PLAYER_PLAY_DEFAULT = 0,
	GT_MEDIA_PLAYER_PLAY_UDP = 1,
	GT_MEDIA_PLAYER_PLAY_TCP = 2,
	GT_MEDIA_PLAYER_PLAY_HTTP = 3,
} GT_MEDIA_PLAYER_PLAY_OPTION;

/*
typedef struct
{
	int test;
} GtMediaPlayer_t;
 */

int GtMediaPlayer_InitLib(gt_utf8 *license, gt_utf8 *token);

//int GtMediaPlayer_New(GtMediaPlayer_t **mp);

int GtMediaPlayer_Play(int idPlayer, gt_utf8 *strUrl, int option);

int GtMediaPlayer_PlayAudioBuffer(int idPlayer, GtBuffer_t *buffer, int option);

int GtMediaPlayer_Stop(int idPlayer);

//int GtMediaPlayer_Free(int idPlayer);

int GtMediaPlayer_SetVideoCallback(int idPlayer, GtCallbackData_t callbackData);

int GtMediaPlayer_SetAudioCallback(int idPlayer, GtCallbackData_t callbackData);

//int GtMediaPlayer_SetVideoSize(GtMediaPlayer_t *mp, int width, int height);

int GtMediaPlayer_GetVideoBufferSize(int idPlayer);
    
int GtMediaPlayer_GetVideoWidth(int idPlayer);
    
int GtMediaPlayer_GetVideoHeight(int idPlayer);

int GtMediaPlayer_GetAudioBufferSize(int idPlayer);
    
int GtMediaPlayer_GetAudioFrameSize(int idPlayer);

int GtMediaPlayer_GetVideoBuffer(int idPlayer, gt_byte *buf, int bufSize);

int GtMediaPlayer_GetAudioBuffer(int idPlayer, gt_byte *buf, int bufSize);

int GtMediaPlayer_GetAudioSampleRate(int idPlayer);

int GtMediaPlayer_GetAudioChannelNumber(int idPlayer);

int GtMediaPlayer_GetPlayStatus(int idPlayer);

#ifdef __cplusplus
}
#endif

#endif /* __GT_MEDIA_PLAYER_H__ */
