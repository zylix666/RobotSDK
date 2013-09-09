/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_LOG_H__
#define __GT_LOG_H__

#include "gt_lib.h"

#if defined(GT_CONFIG_ANDROID)
    #include <android/log.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(GT_CONFIG_ANDROID)
    #define GT_LOG_TAG "GtLib"

    #if defined(GT_CONFIG_DEBUG)
        #define GT_LOG_TAG "GtLib"
        #define GT_LOGI(...) __android_log_print(ANDROID_LOG_INFO, GT_LOG_TAG, __VA_ARGS__);
        #define GT_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, GT_LOG_TAG, __VA_ARGS__);
    #else
        #define GT_LOGI(...) {}
        #define GT_LOGE(...) {}
    #endif
#elif defined(GT_CONFIG_IOS)
    #if defined(GT_CONFIG_DEBUG)
        int GtLog_Log(const char * __restrict format, ...);
        #define GT_LOGI(fmt, ...) GtLog_Log(fmt, ## __VA_ARGS__);
        #define GT_LOGE(fmt, ...) GtLog_Log(fmt, ## __VA_ARGS__);
    #else
        #define GT_LOGI(...) {}
        #define GT_LOGE(...) {}
    #endif
#elif defined(GT_CONFIG_LINUX)
    #if defined(GT_CONFIG_DEBUG)
        int GtLog_Log(const char * __restrict format, ...);
        #define GT_LOGI(fmt, ...) GtLog_Log(fmt, ## __VA_ARGS__);
        #define GT_LOGE(fmt, ...) GtLog_Log(fmt, ## __VA_ARGS__);
    #else
        #define GT_LOGI(...) {}
        #define GT_LOGE(...) {}
    #endif
#else
    #error OS is not defined!
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GT_LOG_H__ */
