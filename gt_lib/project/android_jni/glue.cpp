/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
*/

#include "glue.h"

#include <opencv2/core/core.hpp>

#ifdef __cplusplus
extern "C"
{
#endif

int Glue_Test() {

	GT_LOGI("Glue_Test");

	cv::Mat testMat = cv::Mat(cv::Matx44d
	(
	    1.0, 0.0, 0.0, 0.0,
	    0.0, 1.0, 0.0, 0.0,
	    0.0, 0.0, 1.0, 0.0,
	    0.0, 0.0, 0.0, 1.0
	));
	cv::Mat testMatInv = testMat.inv();

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
