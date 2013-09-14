/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_image.h"
#include "gt_image_ocv.h"

#include <opencv2/opencv.hpp>

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageOcvIsInit = 0;

int GtImageOcv_InitLib() {
	if (gtImageOcvIsInit == 1) return GT_OK_INIT_ALREADY;
	gtImageOcvIsInit = 1;
	
    return GT_OK;
}

int GtImageOcv_Test(GtImage_t *image) {
	if (gtImageOcvIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE_OCV) return GT_ERROR_MAGIC;

	return GT_OK;
}

int GtImageOcv_SetThreshold(GtImage_t *image, int threshold) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (threshold < 0) return GT_ERROR_PARAMETER_1;
	if (threshold > 255) return GT_ERROR_PARAMETER_1;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	cv::Mat gray_img = cv::Mat::ones(image->height, image->width, CV_8UC1);

	/*
	cv::Mat src_img = cv::Mat::ones(image->height, image->width*3, CV_8UC3);

	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			for(int i = 0; i < 3; i++){
				src_img.at<unsigned char>(y,3*x + i) = (int) image->data[(x + y * image->width) * 3 + i];
			}
		}
	}
	cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	cv::threshold(gray_img, gray_img, 125, 255, 0);
	*/

	int x;
	int y;
	gt_byte *data = image->data;
	int dataSize = image->dataSize;
	int width = image->width;
	int height = image->height;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			gray_img.at<unsigned char>(y,x) = data[(x + y * width)];
		}
	}

	cv::threshold(gray_img, gray_img, threshold, 255, 0);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			data[(x + y * width)] = gray_img.at<unsigned char>(y,x);
		}
	}


	return GT_OK;
}

#ifdef __cplusplus
}
#endif
