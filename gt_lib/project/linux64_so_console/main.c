/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include <gt_lib.h>
#include <gt_http.h>
#include <gt_httpd.h>
#include <gt_matrix.h>

static int callback_test_index(void *httpdPtr, void *requestPtr, void *responsePtr) {

	GT_LOGI("callback_test_index");

	return GT_OK;
}

static int callback_test_index2(void *httpdPtr, void *requestPtr, void *responsePtr) {
	
	GT_LOGI("callback_test_index2");	

	return GT_OK;
}

int main () {
	printf("main ++\n");

	int ret;

	ret = GtLib_InitLib(NULL, NULL);
	if (ret <= 0) {
		printf("GtLib_InitLib = %d\n", ret);
		return 1;
	}

	ret = GtHttpd_InitLib(NULL, NULL);
	if (ret <= 0) {
		GT_LOGE("GtHttpd_InitLib = %d\n", ret);
		return 1;
	}

	GtHashMap_t *hashMap = GtHashMap_New();

	gt_utf8 *url_test_index = (gt_utf8 *) "/test/index";
	gt_utf8 *url_test_index2 = (gt_utf8 *) "/test/index2";

	GtHttpdCallback_t httpdCallback = callback_test_index;
	GtHashMap_Append(hashMap, url_test_index, GtUtf8_GetLength(url_test_index), &httpdCallback, sizeof(GtHttpdCallback_t));
	httpdCallback = callback_test_index2;
	GtHashMap_Append(hashMap, url_test_index2, GtUtf8_GetLength(url_test_index2), &httpdCallback, sizeof(GtHttpdCallback_t));

	httpdCallback = NULL;
	GtHashMap_Get(hashMap, url_test_index, GtUtf8_GetLength(url_test_index), &httpdCallback, sizeof(GtHttpdCallback_t));

	//httpdCallback = callback_test_index;

	GT_LOGI("callback_test_index = %d", callback_test_index);
	GT_LOGI("callback_test_index2 = %d", callback_test_index2);

	GT_LOGI("httpdCallback = %d", httpdCallback);

	httpdCallback(NULL, NULL, NULL);

	GtHashMap_Free(hashMap);

	//GtHttpd_t *httpd = GtHttpd_New();

	//GtHttpd_Start(httpd);
	
	GtMatrix_t *matrix = GtMatrix_New();

	//GtMatrix_Init(matrix, 20, 10);

	GtMatrix_Free(matrix);

	GT_LOGI("main --");
	return 0;
}
