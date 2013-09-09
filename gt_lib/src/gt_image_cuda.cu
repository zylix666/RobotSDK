/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */


#include "gt_lib.h"
#include "gt_image.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageCudaIsInit = 0;

int GtImageCuda_InitLib(gt_utf8 *license, gt_utf8 *token) {
	if (gtImageCudaIsInit == 1) return GT_OK_INIT_ALREADY;
	gtImageCudaIsInit = 1;

	// Init CUDA device.
	unsigned char *deviceMemA;
	cudaMalloc((void**)&deviceMemA, 1024 * sizeof(unsigned char));
	cudaFree(deviceMemA);
	
	return GT_OK;
}

int GtImageCuda_Test() {
	if (gtImageCudaIsInit == 0) return GT_ERROR_NOT_INIT;

	return GT_OK;
}

__global__ void gtImageCuda_SetThreshold(unsigned char *deviceMemA, unsigned char *deviceMemB, int width, int height, int threshold) {
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	
	int shift = x + y * width;

	if ((x < width) && (y < height)) {
		if (deviceMemA[shift] >= threshold) {
			deviceMemB[shift] = 255;
		} else {
			deviceMemB[shift] = 0;
		}
	}
}

int GtImageCuda_SetThreshold(GtImage_t *image, int threshold) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (threshold < 0) return GT_ERROR_PARAMETER_1;
	if (threshold > 255) return GT_ERROR_PARAMETER_1;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	unsigned char *deviceMemA;
	unsigned char *deviceMemB;

	cudaMalloc((void**)&deviceMemA, image->dataSize * sizeof(unsigned char));
	cudaMemcpy(deviceMemA, image->data, image->dataSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&deviceMemB, image->dataSize * sizeof(unsigned char));
	
	// 1 block = 32 x 32 = 1024 threads.
	dim3 block(32, 32);

	// 1 grid = X x Y blocks. ceil is the smallest integral value that is not less than x.
	dim3 grid((size_t)(ceil( ((float)image->width)/ ((float)block.x) )),(size_t)(ceil( ((float)image->height)/ ((float)block.y) ))); 
	
	gtImageCuda_SetThreshold<<<grid,block>>>(deviceMemA, deviceMemB, image->width, image->height, threshold);

	cudaMemcpy(image->data , deviceMemB, image->dataSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);

	cudaFree(deviceMemA);
	cudaFree(deviceMemB);

	return GT_OK;
}

__global__ void gtImageCuda_SetBrightness(unsigned char *deviceMemA, unsigned char *deviceMemB, int width, int height, int brightness) {
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int shift = x + y * width;

	int temp;
	if ((x < width) && (y < height)) {
        temp = deviceMemA[shift] + brightness;
		if (temp > 255) {
			deviceMemB[shift] = 255;
		} else if (temp < 0) {
			deviceMemB[shift] = 0;
		} else {
			deviceMemB[shift] = temp;
		}
	}
}

int GtImageCuda_SetBrightness(GtImage_t *image, int brightness) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	unsigned char *deviceMemA;
	unsigned char *deviceMemB;

	cudaMalloc((void**)&deviceMemA, image->dataSize * sizeof(unsigned char));
	cudaMemcpy(deviceMemA, image->data, image->dataSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&deviceMemB, image->dataSize * sizeof(unsigned char));
	
	// 1 block = 32 x 32 = 1024 threads.
	dim3 block(32, 32);

	// 1 grid = X x Y blocks. ceil is the smallest integral value that is not less than x.
	dim3 grid((size_t)(ceil( ((float)image->width)/ ((float)block.x) )),(size_t)(ceil( ((float)image->height)/ ((float)block.y) ))); 
	
	gtImageCuda_SetBrightness<<<grid,block>>>(deviceMemA, deviceMemB, image->width, image->height, brightness);

	cudaMemcpy(image->data , deviceMemB, image->dataSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);

	cudaFree(deviceMemA);
	cudaFree(deviceMemB);

	return GT_OK;
}

int GtImageCuda_GetCorrelation(GtImage_t *imageRoi, GtImage_t *imagePattern, GtImage_t *imageCorrelation) {
	if (imageRoi == NULL) return GT_ERROR_PARAMETER_0;
	if (imageRoi->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (imagePattern == NULL) return GT_ERROR_PARAMETER_1;
	if (imagePattern->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_1;
	if (imageCorrelation == NULL) return GT_ERROR_PARAMETER_2;
	if (imageCorrelation->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_2;
	
	if (imageRoi->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	if (imagePattern->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	if (imageRoi->width < imagePattern->width) {
		return GT_ERROR_PARAMETER_1;
	}

	if (imageRoi->height < imagePattern->height) {
		return GT_ERROR_PARAMETER_1;
	}

	// TODO

	return GT_OK;
}

__global__ void gtImageCuda_SetContrast(unsigned char *deviceMemA, unsigned char *deviceMemB, int width, int height, float contrast) {
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int shift = x + y * width;

	float temp;
	if((x < width) && (y < height)) {
       temp = deviceMemA[shift] * contrast;
		if (temp > 255) {
			deviceMemB[shift] = 255;
		} else if (temp < 0) {
			deviceMemB[shift] = 0;
		} else {
			deviceMemB[shift] = (unsigned char)temp;
		}
	}
}


int GtImageCuda_SetContrast(GtImage_t *image, float contrast) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	unsigned char *deviceMemA;
	unsigned char *deviceMemB;

	cudaMalloc((void**)&deviceMemA, image->dataSize * sizeof(unsigned char));
	cudaMemcpy(deviceMemA, image->data, image->dataSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&deviceMemB, image->dataSize * sizeof(unsigned char));

	// 1 block = 32 x 32 = 1024 threads.
	dim3 block(32, 32);

	// 1 grid = X x Y blocks. ceil is the smallest integral value that is not less than x.
	dim3 grid((size_t)(ceil( ((float)image->width)/ ((float)block.x) )),(size_t)(ceil( ((float)image->height)/ ((float)block.y) )));

	gtImageCuda_SetContrast<<<grid,block>>>(deviceMemA, deviceMemB, image->width, image->height, contrast);

	cudaMemcpy(image->data , deviceMemB, image->dataSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);

	cudaFree(deviceMemA);
	cudaFree(deviceMemB);

	return GT_OK;
}


__global__ void gtImageCuda_Invert(unsigned char *deviceMemA, unsigned char *deviceMemB, int width, int height) {
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int shift = x + y * width;

	if((x < width) && (y < height))
		deviceMemB[shift] = 255 - abs(deviceMemA[shift] % 255);
}


int GtImageCuda_Invert(GtImage_t *image) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	unsigned char *deviceMemA;
	unsigned char *deviceMemB;

	cudaMalloc((void**)&deviceMemA, image->dataSize * sizeof(unsigned char));
	cudaMemcpy(deviceMemA, image->data, image->dataSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&deviceMemB, image->dataSize * sizeof(unsigned char));

	// 1 block = 32 x 32 = 1024 threads.
	dim3 block(32, 32);

	// 1 grid = X x Y blocks. ceil is the smallest integral value that is not less than x.
	dim3 grid((size_t)(ceil( ((float)image->width)/ ((float)block.x) )),(size_t)(ceil( ((float)image->height)/ ((float)block.y) )));

	gtImageCuda_Invert<<<grid,block>>>(deviceMemA, deviceMemB, image->width, image->height);

	cudaMemcpy(image->data , deviceMemB, image->dataSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);

	cudaFree(deviceMemA);
	cudaFree(deviceMemB);

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
