/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_log.h"

#if defined(GT_CONFIG_ANDROID)
	#include <android/log.h>
#endif // GT_CONFIG_ANDROID

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtLogIsInit = 0;

int GtLog_InitLib() {
    if (gtLogIsInit == 1) {
        return GT_OK_INIT_ALREADY;
    }
    gtLogIsInit = 1;
    
    return GT_OK;
}

int GtLog_Log(const char * __restrict format, ...) {
    if (format == NULL) return GT_ERROR_PARAMETER_0;
    
#if defined(GT_CONFIG_IOS)
    va_list args;
    va_start(args,format);
    vfprintf(stdout, format, args);
    //printf(format, args) ;
    va_end(args);
    fprintf(stdout, "\n");
    return 1;
#elif defined(GT_CONFIG_ANDROID)
    va_list args;
    va_start(args,format);
    __android_log_print(ANDROID_LOG_INFO, GT_LOG_TAG, format, args);
    va_end(args);
    return 1;
#elif defined(GT_CONFIG_LINUX)
    va_list args;
    va_start(args,format);
    vfprintf(stdout, format, args);
    //printf(format, args) ;
    va_end(args);
    fprintf(stdout, "\n");
    return 1;
#else
    #error OS is undefined!
#endif
}
    
#ifdef __cplusplus
}
#endif
