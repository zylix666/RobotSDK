/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_CONFIG_H__
#define __GT_CONFIG_H__

#ifdef __cplusplus
extern "C" 
{
#endif

#define GT_CONFIG_DEBUG 1

//#define GT_CONFIG_LINUX 1
#define GT_CONFIG_ANDROID 1
//#define GT_CONFIG_IOS 1
//#define GT_CONFIG_WINDOWS 1

#define GT_CONFIG_32BIT 1
//#define GT_CONFIG_64BIT 1

//#define GT_CONFIG_X86 1
#define GT_CONFIG_ARM 1
//#define GT_CONFIG_SH4 1

#define GT_CONFIG_GCC 1
//#define GT_CONFIG_VC 1

#define GT_CONFIG_PAGE_SIZE 4096

//#define GT_CONFIG_MEDIA_PLAYER 1
//#define GT_CONFIG_MEDIA_PLAYER_FFMPEG_PACKET_LOST_COUNTER 1
//#define GT_CONFIG_MEDIA_PLAYER_DEFAULT_TCP 1

//#define GT_CONFIG_CUDA 1

//#define GT_CONFIG_OCL 1

#ifdef __cplusplus
}
#endif

#endif // __GT_CONFIG_H__
