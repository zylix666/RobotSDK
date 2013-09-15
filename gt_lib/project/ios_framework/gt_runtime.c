/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_runtime.h"

#if defined(GT_CONFIG_ANDROID)
	#include <cpu-features.h>
#endif // GT_CONFIG_ANDROID

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtRuntimeIsInit = 0;

int GtRuntime_InitLib() {
	if (gtRuntimeIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtRuntimeIsInit = 1;
	
	return GT_OK;
}

int GtRuntime_GetCpuCount() {
#if defined(GT_CONFIG_ANDROID)
	return android_getCpuCount();
#elif defined(GT_CONFIG_IOS)
	return GT_ERROR_NOT_SUPPORT;
#elif defined(GT_CONFIG_LINUX)
	return GT_ERROR_NOT_SUPPORT;
#else
	#error OS is not defined!
#endif
}

#ifdef __cplusplus
}
#endif
