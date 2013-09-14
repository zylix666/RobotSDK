/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_lib.h"

int GtLog_InitLib();
int GtMem_InitLib();
int GtUtf8_InitLib();
int GtString_InitLib();
int GtSemaphore_InitLib();
int GtLock_InitLib();
int GtRuntime_InitLib();
int GtBuffer_InitLib();
int GtHashMap_InitLib();
int GtArray_InitLib();
int GtFifo_InitLib();
int GtStack_InitLib();
int GtHttp_InitLib();
int GtMatrix_InitLib();
int GtColor_InitLib();

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtLibIsInit = 0;
   
GT_API int GtLib_InitLib() {
	if (gtLibIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtLibIsInit = 1;
	
	//printf("GtLib_InitLib ++\n");

	/* Init core libraries. ++ */
	int ret;

	ret = GtLog_InitLib();
	if (ret < 0) {
		printf("GtLog_InitLib ERROR!\n");
		return ret;
	}

	ret = GtMem_InitLib();
	if (ret < 0) {
		GT_LOGE("GtUtf8_InitLib ERROR!");
		return ret;
	}

	ret = GtUtf8_InitLib();
	if (ret < 0) {
		GT_LOGE("GtUtf8_InitLib ERROR!");
		return ret;
	}

	ret = GtString_InitLib();
	if (ret < 0) {
		GT_LOGE("GtString_InitLib ERROR!");
		return ret;
	}

	ret = GtSemaphore_InitLib();
	if (ret < 0) {
		GT_LOGE("GtSemaphore_InitLib ERROR!");
		return ret;
	}

	ret = GtLock_InitLib();
	if (ret < 0) {
		GT_LOGE("GtLock_InitLib ERROR!");
		return ret;
	}

	ret = GtRuntime_InitLib();
	if (ret < 0) {
		GT_LOGE("GtRuntime_InitLib ERROR!");
		return ret;
	}

	ret = GtBuffer_InitLib();
	if (ret < 0) {
		GT_LOGE("GtBuffer_InitLib ERROR!");
		return ret;
	}

	ret = GtHashMap_InitLib();
	if (ret < 0) {
		GT_LOGE("GtHashMap_InitLib ERROR!");
		return ret;
	}

	ret = GtFile_InitLib();
	if (ret < 0) {
		GT_LOGE("GtFile_InitLib ERROR!");
		return ret;
	}

	ret = GtTimerUs_InitLib();
	if (ret < 0) {
		GT_LOGE("GtTimerUs_InitLib ERROR!");
		return ret;
	}

	ret = GtArray_InitLib();
	if (ret < 0) {
		GT_LOGE("GtArray_InitLib ERROR!");
		return ret;
	}

	ret = GtFifo_InitLib();
	if (ret < 0) {
		GT_LOGE("GtFifo_InitLib ERROR!");
		return ret;
	}

	ret = GtStack_InitLib();
	if (ret < 0) {
		GT_LOGE("GtStack_InitLib ERROR!");
		return ret;
	}

	ret = GtHttp_InitLib();
	if (ret < 0) {
		GT_LOGE("GtHttp_InitLib ERROR!");
		return ret;
	}

	ret = GtMatrix_InitLib();
	if (ret < 0) {
		GT_LOGE("GtMatrix_InitLib ERROR!");
		return ret;
	}

	ret = GtColor_InitLib();
	if (ret < 0) {
		GT_LOGE("GtColor_InitLib ERROR!");
		return ret;
	}
	/* Init core libraries. -- */

	//GT_LOGI("GtLib_InitLib --");
	return GT_OK;
}

#ifdef __cplusplus
}
#endif
