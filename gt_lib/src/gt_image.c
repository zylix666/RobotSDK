/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_image.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtImageIsInit = 0;

int GtImage_InitLib() {
	if (gtImageIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtImageIsInit = 1;
	
    return GT_OK;
}

GtImage_t *GtImage_New() {
	GtImage_t *image;
	if (gtImageIsInit == 0) return NULL;

	image = (GtImage_t *)gt_calloc(1, sizeof(GtImage_t));
	if (image == NULL) return NULL;

	image->magic = GT_MAGIC_IMAGE;

	return image;
}

int GtImage_Free(GtImage_t *image) {
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	image->magic = GT_MAGIC_UNKNOWN;
	
	gt_free(image->data);

	GtLock_Free(image->lock);

	gt_free(image);

	return GT_OK;
}

int GtImage_Lock(GtImage_t *image) {
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	if (image->lock == NULL) {
		image->lock = GtLock_New();
		if (image->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(image->lock);
}

int GtImage_Unlock(GtImage_t *image) {
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	return GtLock_Unlock(image->lock);
}

int GtImage_InitImage(GtImage_t *image, int width, int height, int colorMode) {
	
	int dataSize = 0;
	
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (width <= 0) return GT_ERROR_PARAMETER_1;
	if (height <= 0) return GT_ERROR_PARAMETER_2;
	if (colorMode <= 0) return GT_ERROR_PARAMETER_3;
	if (colorMode >= GT_COLOR_MODE_COUNT) return GT_ERROR_PARAMETER_3;

	gt_free(image->data);
	image->data = NULL;
	image->dataSize = 0;
	image->colorMode = GT_COLOR_MODE_UNKNOWN;
	image->width = 0;
	image->height = 0;


	if (colorMode == GT_COLOR_MODE_GRAY8) {
		dataSize = width * height;
		image->data = gt_calloc(1, dataSize);
	} else if (colorMode == GT_COLOR_MODE_RGB888) {
		dataSize = width * height * 3;
		image->data = gt_calloc(1, dataSize);
	} else if (colorMode == GT_COLOR_MODE_INT32) {
		dataSize = width * height * 4;
		image->data = gt_calloc(1, dataSize);
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	if (image->data == NULL) {
		return GT_ERROR_NO_MEMORY;
	}

	image->dataSize = dataSize;
	image->colorMode = colorMode;
	image->width = width;
	image->height = height;

	return GT_OK;
}

int GtImage_GetColorMode(GtImage_t *image) {
	if (image == NULL) return GT_ERROR_NULL_POINTER;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_MAGIC;

	return image->colorMode;
}

int GtImage_SetColorMode(GtImage_t *image, int colorMode) {
		
		gt_byte gray8;
		int gray;
		int shiftRgb888;
		int shiftGray8;
		int x;
		int y;

	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (colorMode <= 0) return GT_ERROR_PARAMETER_1;
	if (colorMode >= GT_COLOR_MODE_COUNT) return GT_ERROR_PARAMETER_1;

	if (image->colorMode == colorMode) {
		return GT_OK_DONE_ALREADY;
	}

	if (image->colorMode == GT_COLOR_MODE_RGB888 && colorMode == GT_COLOR_MODE_GRAY8) {
		int dataSize = image->width * image->height;
		gt_byte *data = gt_calloc(1, dataSize);
		if (data == NULL) {
			return GT_ERROR_NO_MEMORY;
		}


		for (y = 0; y < image->height; y ++) {
			for (x = 0; x < image->width; x ++) {
				shiftRgb888 = (x + y * image->width) * 3;
				shiftGray8 = x + y * image->width;

				gray  = (int)image->data[shiftRgb888];
				gray += (int)image->data[shiftRgb888 + 1];
				gray += (int)image->data[shiftRgb888 + 2];
				gray = gray / 3;

				data[shiftGray8] = (gt_byte)gray;
			}
		}

		gt_free(image->data);
		image->data = data;
		image->dataSize = dataSize;
		image->colorMode = colorMode;
	} else if (image->colorMode == GT_COLOR_MODE_GRAY8 && colorMode == GT_COLOR_MODE_RGB888) {
		int dataSize = image->width * image->height * 3;
		gt_byte *data = gt_calloc(1, dataSize);
		if (data == NULL) {
			return GT_ERROR_NO_MEMORY;
		}


		for (y = 0; y < image->height; y ++) {
			for (x = 0; x < image->width; x ++) {
				shiftRgb888 = (x + y * image->width) * 3;
				shiftGray8 = x + y * image->width;
				
				gray8 = image->data[shiftGray8];

				data[shiftRgb888] = gray8;
				data[shiftRgb888 + 1] = gray8;
				data[shiftRgb888 + 2] = gray8;
			}
		}

		gt_free(image->data);
		image->data = data;
		image->dataSize = dataSize;
		image->colorMode = colorMode;
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	return GT_OK;
}

int GtImage_Fill(GtImage_t *image, GtColor_t *color) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (color == NULL) return GT_ERROR_PARAMETER_1;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_PARAMETER_1;

	if (image->colorMode != color->mode) {
		return GT_ERROR_COLOR_MODE;
	}

	if (image->colorMode == GT_COLOR_MODE_RGB888) {
		int shiftRgb888;
		int x;
		int y;
		for (y = 0; y < image->height; y ++) {
			for (x = 0; x < image->width; x ++) {
				shiftRgb888 = (x + y * image->width) * 3;

				image->data[shiftRgb888] = color->red8;
				image->data[shiftRgb888 + 1] = color->green8;
				image->data[shiftRgb888 + 2] = color->blue8;
			}
		}

		//GT_LOGI("GtImage_Fill w = %d, h = %d", image->width, image->height);
	} else if (image->colorMode == GT_COLOR_MODE_GRAY8) {
		gt_memset(image->data, color->gray8, image->dataSize);
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	return GT_OK;
}

int GtImage_GetPixelColor(GtImage_t *image, int x, int y, GtColor_t *color) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (x < 0) return GT_ERROR_PARAMETER_1;
	if (x >= image->width) return GT_ERROR_PARAMETER_1;
	if (y < 0) return GT_ERROR_PARAMETER_2;
	if (y >= image->height) return GT_ERROR_PARAMETER_2;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_PARAMETER_3;

	color->mode = GT_COLOR_MODE_UNKNOWN;

	if (image->colorMode == GT_COLOR_MODE_RGB888) {
		int shiftRgb888 = (x + y * image->width) * 3;
		color->red8    = image->data[shiftRgb888];
		color->green8  = image->data[shiftRgb888 + 1];
		color->blue8   = image->data[shiftRgb888 + 2];
	} else if (image->colorMode == GT_COLOR_MODE_GRAY8) {
		int shiftGray8 = x + y * image->width;
		color->gray8    = image->data[shiftGray8];
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	color->mode = image->colorMode;

	return GT_OK;
}

int GtImage_DrawPixel(GtImage_t *image, int x, int y, GtColor_t *color) {
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (x < 0) return GT_ERROR_PARAMETER_1;
	if (x >= image->width) return GT_ERROR_PARAMETER_1;
	if (y < 0) return GT_ERROR_PARAMETER_2;
	if (y >= image->height) return GT_ERROR_PARAMETER_2;
	if (color->magic != GT_MAGIC_COLOR) return GT_ERROR_PARAMETER_3;

	if (image->colorMode != color->mode) {
		return GT_ERROR_COLOR_MODE;
	}

	if (image->colorMode == GT_COLOR_MODE_RGB888) {
		int shiftRgb888 = (x + y * image->width) * 3;
		image->data[shiftRgb888] = color->red8;
		image->data[shiftRgb888 + 1] = color->green8;
		image->data[shiftRgb888 + 2] = color->blue8;
	} else if (image->colorMode == GT_COLOR_MODE_GRAY8) {
		int shiftGray8 = x + y * image->width;
		image->data[shiftGray8] = color->gray8;
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	return GT_OK;
}

int GtImage_SetThreshold(GtImage_t *image, int threshold) {
		int i;
		int dataSize;

	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (threshold < 0) return GT_ERROR_PARAMETER_1;
	if (threshold > 255) return GT_ERROR_PARAMETER_1;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}


	dataSize = image->dataSize;
	for (i = 0; i < dataSize; i ++) {
		if (image->data[i] >= threshold) {
			image->data[i] = 255;
		} else {
			image->data[i] = 0;
		}
	}

	return GT_OK;
}

int GtImage_SetBrightness(GtImage_t *image, int brightness) {

		int i;
	int dataSize;
	int temp;


	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}

	dataSize = image->dataSize;
	
	for (i = 0; i < dataSize; i ++) {
        temp = image->data[i] + brightness;

		if (temp > 255) {
			image->data[i] = 255;
		} else if (temp < 0) {
			image->data[i] = 0;
		} else {
			image->data[i] = temp;
		}
	}

	return GT_OK;
}

int GtImage_GetCorrelation(GtImage_t *imageRoi, GtImage_t *imagePattern, GtImage_t *imageCorrelation) {
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

int GtImage_Load(GtImage_t *image, gt_byte *buffer, int bufferLength, int colorMode, int width, int height) {

	int length = 0;
		int x;
		int y;
		int imageDataShift;
		int bufferShift;
		unsigned char byte0;
		unsigned char byte1;
		unsigned char r;
		unsigned char g;
		unsigned char b;



	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (buffer == NULL) return GT_ERROR_PARAMETER_1;
	if (bufferLength <= 0) return GT_ERROR_PARAMETER_2;
	if (colorMode <= 0) return GT_ERROR_PARAMETER_3;
	if (width <= 0) return GT_ERROR_PARAMETER_4;
	if (height <= 0) return GT_ERROR_PARAMETER_5;


	if (image->colorMode == GT_COLOR_MODE_RGB888 && colorMode == GT_COLOR_MODE_RGB565) {
		length = width * height * 2;
		if (bufferLength < length) {
			return GT_ERROR_PARAMETER_2;
		}


		for (y = 0; y < height; y ++) {
			for (x = 0; x < width; x ++) {
				imageDataShift = (x + y * width) * 3;
				bufferShift = (x + y * width) * 2;

				byte0 = buffer[bufferShift];
				byte1 = buffer[bufferShift + 1];

				r = (byte1 & 0xF8);
				g = (byte0 & 0xD0) >> 3;
				g += (byte1 & 0x7) << 5;
				b = (byte0 & 0x1F) << 3;

				image->data[imageDataShift] = r;
				image->data[imageDataShift + 1] = g;
				image->data[imageDataShift + 2] = b;
			}
		}
		return length;
	}

	if (image->colorMode == GT_COLOR_MODE_RGB888 && colorMode == GT_COLOR_MODE_ARGB8888) {
		length = width * height * 4;
		if (bufferLength < length) {
			return GT_ERROR_PARAMETER_2;
		}


		for (y = 0; y < height; y ++) {
			for (x = 0; x < width; x ++) {
				imageDataShift = (x + y * width) * 3;
				bufferShift = (x + y * width) * 4;

				image->data[imageDataShift]     = buffer[bufferShift];
				image->data[imageDataShift + 1] = buffer[bufferShift + 1];
				image->data[imageDataShift + 2] = buffer[bufferShift + 2];
			}
		}
		return length;
	}

	return GT_ERROR_NO_MATCH;
}

int GtImage_Save(GtImage_t *image, gt_byte *buffer, int bufferLength, int colorMode, int width, int height) {

		int x;
		int y;
		int imageDataShift;
		int bufferShift;
		unsigned char byte0;
		unsigned char byte1;
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char temp;
		int length = 0;

	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (buffer == NULL) return GT_ERROR_PARAMETER_1;
	if (bufferLength <= 0) return GT_ERROR_PARAMETER_2;
	if (colorMode <= 0) return GT_ERROR_PARAMETER_3;
	if (width <= 0) return GT_ERROR_PARAMETER_4;
	if (height <= 0) return GT_ERROR_PARAMETER_5;



	if (image->colorMode == GT_COLOR_MODE_RGB888 && colorMode == GT_COLOR_MODE_RGB565) {
		length = width * height * 2;

		if (bufferLength < length) {
			return GT_ERROR_PARAMETER_2;
		}


		for (y = 0; y < height; y ++) {
			for (x = 0; x < width; x ++) {
				imageDataShift = (x + y * width) * 3;
				bufferShift = (x + y * width) * 2;

				r = image->data[imageDataShift];
				g = image->data[imageDataShift + 1];
				b = image->data[imageDataShift + 2];

				byte0 = (b & 0xF8) >> 3;
				byte0 += (g & 0x1C) << 3;

				byte1 = (g & 0xD0) >> 5;
				byte1 += r & 0xF8;

				buffer[bufferShift] = byte0;
				buffer[bufferShift + 1] = byte1;
			}
		}
		return length;
	}

	if (image->colorMode == GT_COLOR_MODE_RGB888 && colorMode == GT_COLOR_MODE_ARGB8888) {
		length = width * height * 4;

		if (bufferLength < length) {
			return GT_ERROR_PARAMETER_2;
		}

		for (y = 0; y < height; y ++) {
			for (x = 0; x < width; x ++) {
				imageDataShift = (x + y * width) * 3;
				bufferShift = (x + y * width) * 4;

				buffer[bufferShift] = image->data[imageDataShift];
				buffer[bufferShift + 1] = image->data[imageDataShift + 1];
				buffer[bufferShift + 2] = image->data[imageDataShift + 2];
				buffer[bufferShift + 3] = 255;
			}
		}
		return length;
	}

	if (image->colorMode == GT_COLOR_MODE_GRAY8 && colorMode == GT_COLOR_MODE_ARGB8888) {
			length = width * height * 4;

			if (bufferLength < length) {
				return GT_ERROR_PARAMETER_2;
			}


			for (y = 0; y < height; y ++) {
				for (x = 0; x < width; x ++) {
					imageDataShift = x + y * width;
					bufferShift = (x + y * width) * 4;

					temp = image->data[imageDataShift];
					buffer[bufferShift] = temp;
					buffer[bufferShift + 1] = temp;
					buffer[bufferShift + 2] = temp;
					buffer[bufferShift + 3] = 255;
				}
			}
			return length;
		}

	return GT_ERROR_PARAMETER_3;
}

int GtImage_SetContrast(GtImage_t *image, float contrast) {

		int i;
	int dataSize = image->dataSize;
	float temp;

	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}


	for (i = 0; i < dataSize; i ++) {
       temp = image->data[i] * contrast;

		if (temp > 255) {
			image->data[i] = 255;
		} else if (temp < 0) {
			image->data[i] = 0;
		} else {
			image->data[i] = (unsigned char)temp;
		}
	}

	return GT_OK;
}


int GtImage_Invert(GtImage_t *image) {

		int i;
	int dataSize = image->dataSize;

	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;

	if (image->colorMode != GT_COLOR_MODE_GRAY8) {
		return GT_ERROR_COLOR_MODE;
	}


	for (i = 0; i < dataSize; i ++)
       image->data[i] = 255 - abs(image->data[i] % 255);

	return GT_OK;
}

int gt_image_EmptyGabor(GtImage_t *image)
{
  gt_int i;
  gt_int x;
 // static GtSpinLock_t lock = GT_SPIN_LOCK_UNLOCKED;

  //if(GtImage_Magic(image) != GT_SUCCESS) return GT_ERR_MAGIC;

  //GtSpinLock_Lock(&lock);

  // Free memory
  for(x = 0; x < image->gabor_width; x ++) {
    if(image->gabor_pp_b_data[x] != NULL)
      gt_free(image->gabor_pp_b_data[x]);
    if(image->gabor_pp_i_data[x] != NULL)
      gt_free(image->gabor_pp_i_data[x]);
    if(image->gabor_pp_b_buf[x] != NULL)
      gt_free(image->gabor_pp_b_buf[x]);
    if(image->gabor_pp_i_buf[x] != NULL)
      gt_free(image->gabor_pp_i_buf[x]);
  }

  if(image->gabor_pp_b_data != NULL)
    gt_free(image->gabor_pp_b_data);
  if(image->gabor_pp_i_data != NULL)
    gt_free(image->gabor_pp_i_data);
  if(image->gabor_pp_b_buf != NULL)
    gt_free(image->gabor_pp_b_buf);
  if(image->gabor_pp_i_buf != NULL)
    gt_free(image->gabor_pp_i_buf);

  image->gabor_pp_b_data = NULL;
  image->gabor_pp_i_data = NULL;
  image->gabor_pp_b_buf  = NULL;
  image->gabor_pp_i_buf  = NULL;


  for(i = 0; i < image->gabor_dir_no; i ++)
  {
    for(x = 0; x <  image->gabor_filter_size; x ++)
    {
      if(image->gabor_coef[i][x] != NULL)
        gt_free(image->gabor_coef[i][x]);
    }

    if(image->gabor_coef[i] != NULL)
    gt_free(image->gabor_coef[i]);
  }

  if(image->gabor_coef != NULL)
  gt_free(image->gabor_coef);
  image->gabor_coef = NULL;

  image->gabor_width = 0;
  image->gabor_height = 0;
  image->gabor_dir_no = 0;
  image->gabor_block_size = 0;
  image->gabor_dir_avg_size = 0;
  image->gabor_filter_size = 0;
  image->gabor_freq = 0;
  image->gabor_delta_x = 0;
  image->gabor_delta_y = 0;

  //GtSpinLock_Unlock(&lock);

  return GT_OK;
}

int gt_image_GaborCofficients(GtImage_t *image, gt_int gabor_dir_no, gt_int gabor_block_size, gt_int gabor_dir_avg_size, gt_int gabor_filter_size, gt_double gabor_freq, gt_double gabor_delta_x, gt_double gabor_delta_y)
{
  gt_int i;
  gt_int j;
  gt_int x;
//  gt_int y;

  gt_double dx,dy;
  gt_double value, ang, ang_scale;
  gt_double gabor_delta_x_2;
  gt_double gabor_delta_y_2;
  gt_int    dno;
  gt_int center;

  //if(GtImage_Magic(image) != GT_OK) return GT_ERROR_MAGIC;
  //if(image->type != GT_IMAGE_GRAY8) return GT_ERROR_PARAMETER_WRONG;

  /*
  if((image->width == gt_int_to_size(image->gabor_width)) &&
     (image->height == gt_int_to_size(image->gabor_height)) &&
     (gabor_dir_no == image->gabor_dir_no) &&
     (gabor_block_size == image->gabor_block_size) &&
     (gabor_dir_avg_size == image->gabor_dir_avg_size) &&
     (gabor_filter_size == image->gabor_filter_size) &&
     (gabor_freq == image->gabor_freq) &&
     (gabor_delta_x == image->gabor_delta_x) &&
     (gabor_delta_y == image->gabor_delta_y))
  {
    GT_LOGI("gt_image_GaborCofficients: Skip" GT_NL);
    //GtSpinLock_Unlock(&lock);

    return GT_OK;
  }
  */

  gt_image_EmptyGabor(image);

  image->gabor_width = image->width;
  image->gabor_height = image->height;
  image->gabor_dir_no = gabor_dir_no;
  image->gabor_block_size = gabor_block_size;
  image->gabor_dir_avg_size = gabor_dir_avg_size;
  image->gabor_filter_size = gabor_filter_size;
  image->gabor_freq = gabor_freq;
  image->gabor_delta_x = gabor_delta_x;
  image->gabor_delta_y = gabor_delta_y;

  // Allocate memory
  image->gabor_pp_i_buf = (gt_int **)gt_malloc(image->gabor_width * sizeof(gt_int *));
  image->gabor_pp_i_data = (gt_int **)gt_malloc(image->gabor_width * sizeof(gt_int *));
  image->gabor_pp_b_buf = (gt_byte **)gt_malloc(image->gabor_width * sizeof(gt_byte *));
  image->gabor_pp_b_data = (gt_byte **)gt_malloc(image->gabor_width * sizeof(gt_byte *));

	for(x = 0; x < image->gabor_width; x ++)
	{
    image->gabor_pp_i_buf[x] = (gt_int *)gt_malloc(image->gabor_height * sizeof(gt_int));
    gt_memset(image->gabor_pp_i_buf[x], 0, image->gabor_height * sizeof(gt_int));

    image->gabor_pp_i_data[x] = (gt_int *)gt_malloc(image->gabor_height * sizeof(gt_int));
    gt_memset(image->gabor_pp_i_data[x], 0, image->gabor_height * sizeof(gt_int));

    image->gabor_pp_b_buf[x] = (gt_byte *)gt_malloc(image->gabor_height * sizeof(gt_byte));
    gt_memset(image->gabor_pp_b_buf[x], 0, image->gabor_height * sizeof(gt_byte));

		image->gabor_pp_b_data[x] = (gt_byte *)gt_malloc(image->gabor_height * sizeof(gt_byte));
		gt_memset(image->gabor_pp_b_data[x], 0, image->gabor_height * sizeof(gt_byte));
	}

  image->gabor_coef = (gt_int ***)gt_malloc(image->gabor_dir_no * sizeof(gt_int **));
  for(i = 0; i < image->gabor_dir_no; i ++)
  {
    image->gabor_coef[i] = (gt_int **)gt_malloc(gabor_filter_size * sizeof(gt_int *));
    for(x = 0; x < gabor_filter_size; x ++)
    {
      image->gabor_coef[i][x] = (gt_int *)gt_malloc(gabor_filter_size * sizeof(gt_int));
    }
  }

  gabor_delta_x_2 = gabor_delta_x * gabor_delta_x;
  gabor_delta_y_2 = gabor_delta_y * gabor_delta_y;
  center          = gabor_filter_size/2;

	ang_scale = 180.0/(gt_double)gabor_dir_no;
  for(dno = 0; dno < gabor_dir_no; dno ++)
  {
    ang = GT_MATH_PI * dno * ang_scale/180.0;
    for(j = 0; j < gabor_filter_size; j ++)
    {
      for(i = 0; i < gabor_filter_size; i ++)
      {
        dx = (j-center)*sin(ang)+(i-center)*cos(ang);
        dy = (j-center)*cos(ang)-(i-center)*sin(ang);

        // Gabor's formula.
        value = exp(-0.5*(dx*dx/gabor_delta_x_2+dy*dy/gabor_delta_y_2))*cos(2.0*GT_MATH_PI*gabor_freq*dx);
        if(fabs(value) <= 0.01)
        {
          value = 0.0;
        }
        image->gabor_coef[dno][i][j] = (gt_int)(value*1024.0);
      }
    }
  }

  return GT_OK;
}

int gt_image_GaborOrientation(GtImage_t *image, gt_int nAvgSize)
{
  gt_int i,m,n,Cx,Cy;
  gt_int x,y;
  gt_double SumCos,SumSin,NewDir,Vy,Vx,Theata,fAngScale;
  gt_int nTopY,nBottomY,nLeftX,nRightX;
  gt_int nLastY,nLastX,nAngLeft,nAngRight;
  gt_int DirAvg = nAvgSize;

  gt_int m_DirH=image->gabor_width/image->gabor_block_size;
  gt_int m_DirV=image->gabor_height/image->gabor_block_size;

  gt_int **m_ppIDir;
  gt_double **m_ppfDirbuf;
  gt_double **fSin;
  gt_double **fCos;
  gt_int **nBufferH;
  gt_int **nBufferV;
  //static GtSpinLock_t lock = GT_SPIN_LOCK_UNLOCKED;

  //if(GtImage_Magic(image) != GT_OK) return GT_ERROR_MAGIC;
  //if(image->type != GT_IMAGE_GRAY8) return GT_ERROR_PARAMETER_WRONG;

  //GtSpinLock_Lock(&lock);

  // Allocate memory
  m_ppIDir = (gt_int **)gt_malloc(m_DirH*sizeof(gt_int *));
  m_ppfDirbuf = (gt_double **)gt_malloc(m_DirH*sizeof(gt_double *));
  fSin = (gt_double **)gt_malloc(m_DirH*sizeof(gt_double *));
  fCos = (gt_double **)gt_malloc(m_DirH*sizeof(gt_double *));

	for(i=0;i<m_DirH;i++)
	{
    m_ppIDir[i] = (gt_int *)gt_malloc(m_DirV*sizeof(gt_int));
    gt_memset(m_ppIDir[i],0,m_DirV*sizeof(gt_int));

    m_ppfDirbuf[i] = (gt_double *)gt_malloc(m_DirV*sizeof(gt_double));
    gt_memset(m_ppfDirbuf[i],0,m_DirV*sizeof(gt_double));

    fSin[i] = (gt_double *)gt_malloc(m_DirV*sizeof(gt_double));
    gt_memset(m_ppfDirbuf[i],0,m_DirV*sizeof(gt_double));

    fCos[i] = (gt_double *)gt_malloc(m_DirV*sizeof(gt_double));
    gt_memset(m_ppfDirbuf[i],0,m_DirV*sizeof(gt_double));
	}

  nBufferH = (gt_int **)gt_malloc(m_DirH*sizeof(gt_int *));
  nBufferV = (gt_int **)gt_malloc(m_DirH*sizeof(gt_int *));
  for(i = 0; i < m_DirH; i ++)
  {
    nBufferH[i] = (gt_int *)gt_malloc(m_DirV*sizeof(gt_int));
    nBufferV[i] = (gt_int *)gt_malloc(m_DirV*sizeof(gt_int));
    gt_memset(nBufferH[i], 0, m_DirV*sizeof(gt_int));
    gt_memset(nBufferV[i], 0, m_DirV*sizeof(gt_int));
  }



	// Calculate the orientation of each block window
  nLastX = image->gabor_width - image->gabor_block_size;
	nLastY = image->gabor_height - image->gabor_block_size;

	for(y= 0; y < image->gabor_height; y += image->gabor_block_size)
        {
		nTopY = y;
		nBottomY = y+image->gabor_block_size;

		if(y == 0)
                        nTopY++;
		else if(y == nLastY)
			nBottomY--;

		for(x=0;x<image->gabor_width;x+=image->gabor_block_size)
                {
			nLeftX = x;
			nRightX = x+image->gabor_block_size;

			if(x == 0)
				nLeftX++;
			else if(x == nLastX)
				nRightX--;

			for(n=nTopY;n<nBottomY;n++)
                        {
				for(m=nLeftX;m<nRightX;m++)
                                {
nBufferH[m-nLeftX][n-nTopY] =	(int)image->data[(m+1)+image->width*(n-1)]+
                            2*(int)image->data[(m+1)+image->width*(n)]+
                              (int)image->data[(m+1)+image->width*(n+1)]-
                             ((int)image->data[(m-1)+image->width*(n-1)]+
                            2*(int)image->data[(m-1)+image->width*(n)]+
                              (int)image->data[(m-1)+image->width*(n+1)]);

nBufferV[m-nLeftX][n-nTopY] =	(int)image->data[(m-1)+image->width*(n+1)]+
                            2*(int)image->data[(m)+image->width*(n+1)]+
                              (int)image->data[(m+1)+image->width*(n+1)]-
                             ((int)image->data[(m-1)+image->width*(n-1)]+
                            2*(int)image->data[(m)+image->width*(n-1)]+
                              (int)image->data[(m+1)+image->width*(n-1)]);
				}
			}

			Vx=0.0;
                        Vy=0.0;
			for(n=0;n<image->gabor_block_size;n++)
                        {
				for(m=0;m<image->gabor_block_size;m++)
                                {
					Vx += (double)(2*nBufferH[m][n]*nBufferV[m][n]);
					Vy += (double)(nBufferH[m][n]*nBufferH[m][n]-nBufferV[m][n]*nBufferV[m][n]);
				}
			}
			if(Vy == 0.0)
                        {
				Vy = 0.0000000001;
			}
			Theata = 90.0*atan(Vx/Vy)/GT_MATH_PI;
			if(Vy<0.0 && Vx>0.0) {
				Theata += 90.0;
			}
			else if(Vy<0.0 && Vx<0.0) {
				Theata += 90.0;
			}
			else if(Vy>0.0 && Vx<0.0) {
				Theata += 180.0;
			}
			if(Theata >= 180.0) {
				Theata = 0.0;
			}
			m_ppIDir[x/image->gabor_block_size][y/image->gabor_block_size] = (int)Theata;
		}
	}

        // Smooth the orientation of block
        for(y=0;y<m_DirV;y++)
        {
                for(x=0;x<m_DirH;x++)
                {
                        fCos[x][y] = cos(2.0*(double)m_ppIDir[x][y]*GT_MATH_PI/180.0);
                        fSin[x][y] = sin(2.0*(double)m_ppIDir[x][y]*GT_MATH_PI/180.0);
                }
        }
	fAngScale = 180.0/image->gabor_dir_no;

        for(y=0;y<m_DirV;y++)
        {
		nTopY = y-DirAvg;
		nBottomY = y+DirAvg;
		if(nTopY < 0)
                {
			nTopY = 0;
			nBottomY = DirAvg*2;
		}
		if(nBottomY >= m_DirV)
                {
			nBottomY = m_DirV-1;
			nTopY = nBottomY-DirAvg*2;
		}
		for(x=0;x<m_DirH;x++)
                {
			nLeftX = x-DirAvg;
			nRightX = x+DirAvg;
			if(nLeftX < 0)
                        {
				nLeftX = 0;
				nRightX = DirAvg*2;
			}
			if(nRightX >= m_DirH)
                        {
				nRightX = m_DirH-1;
				nLeftX = nRightX-DirAvg*2;
			}
			SumCos=0.0;
                        SumSin=0.0;
			for(m=nTopY;m<=nBottomY;m++)
                        {
				for(n=nLeftX;n<=nRightX;n++)
                                {
					SumCos += fCos[m][n];
					SumSin += fSin[m][n];
				}
			}
			if(SumCos == 0.0) {
				SumCos = (double)0.0000000001;
			}
			NewDir = (double)(0.5*atan(SumSin/SumCos)*180.0/GT_MATH_PI);
			if(SumCos<0.0 && SumSin>0.0)
                        {
				NewDir += 90.0;
			}
			else if(SumCos<0.0 && SumSin<0.0)
                        {
				NewDir += 90.0;
			}
			else if(SumCos>0.0 && SumSin<0.0)
                        {
				NewDir += 180.0;
			}
			if(NewDir >= 180.0)
                        {
				NewDir = 0.0;
			}
			nAngLeft = (int)(NewDir/fAngScale);
			nAngRight = nAngLeft+1;
			if(NewDir-(double)nAngLeft*fAngScale < (double)nAngRight*fAngScale-NewDir)
                        {
				m_ppIDir[x][y] = nAngLeft;
			}
			else {
				if(nAngRight >= image->gabor_dir_no)
                                {
					nAngRight = 0;
				}
				m_ppIDir[x][y] = nAngRight;
			}
			m_ppfDirbuf[x][y] = NewDir;
		}
	}



        // Save to the directional image buffer
        for(y=0;y<(int)image->height;y+=image->gabor_block_size)
        {
	        for(x=0;x<(int)image->width;x+=image->gabor_block_size)
                {
		        Cy = y/image->gabor_block_size;
			Cx = x/image->gabor_block_size;
			for(m=y;m<y+image->gabor_block_size;m++)
                        {
				for(n=x;n<x+image->gabor_block_size;n++)
                                {
					image->gabor_pp_i_data[m][n] = m_ppIDir[Cx][Cy];
				}
			}
		}
	}


        // Free memory
	for(i = 0; i < m_DirH; i ++)
        {
                gt_free(m_ppIDir[i]);
                gt_free(m_ppfDirbuf[i]);
                gt_free(fSin[i]);
                gt_free(fCos[i]);
        }

        for(i = 0; i < m_DirH; i++)
        {
                gt_free(nBufferH[i]);
                gt_free(nBufferV[i]);
        }

        gt_free(m_ppIDir);
        gt_free(m_ppfDirbuf);
        gt_free(nBufferH);
        gt_free(nBufferV);
        gt_free(fSin);
        gt_free(fCos);

        return GT_OK;
}

int GtImage_SetGabor(GtImage_t *image, gt_int gabor_dir_no, gt_int gabor_block_size, gt_int gabor_dir_avg_size, gt_int gabor_filter_size, gt_double gabor_freq, gt_double gabor_delta_x, gt_double gabor_delta_y)
{
  GT_RET_t ret;
  //static GtSpinLock_t lock = GT_SPIN_LOCK_UNLOCKED;

  //if(GtImage_Magic(image) != GT_OK) return GT_ERROR_MAGIC;
  //if(image->type != GT_IMAGE_GRAY8) return GT_ERROR_PARAMETER_WRONG;
//  if(image->width != image->height) return GT_ERROR_PARAMETER_WRONG;

  ret = gt_image_GaborCofficients(image, gabor_dir_no, gabor_block_size, gabor_dir_avg_size, gabor_filter_size, gabor_freq, gabor_delta_x, gabor_delta_y);

  return ret;
}

int GtImage_Gabor(GtImage_t *image)
{
  GT_RET_t ret;
  int m,n;
  unsigned int nBorder;
	int		nPixel;
  unsigned int x,y;
  //static GtSpinLock_t lock = GT_SPIN_LOCK_UNLOCKED;

  //if(GtImage_Magic(image) != GT_OK) return GT_ERROR_MAGIC;
  //if(image->type != GT_IMAGE_GRAY8) return GT_ERROR_PARAMETER_WRONG;
  if(image->gabor_width == 0) return GT_ERROR;
  if(image->gabor_height == 0) return GT_ERROR;
//  if(image->width != image->height) return GT_ERROR_PARAMETER_WRONG;

  nBorder = image->gabor_filter_size / 2;

  for(y = 0; y < (unsigned int)image->gabor_height; y ++)
  {
    for(x = 0; x < (unsigned int)image->gabor_width; x ++)
    {
      image->gabor_pp_b_data[x][y] = image->data[x+y*image->width];
      image->gabor_pp_b_buf[x][y]  = image->data[x+y*image->width];
    }
  }

  ret = gt_image_GaborOrientation(image, image->gabor_dir_avg_size);
//
//  if(ret != GT_OK)
//  {
//    GT_LOGI("GtImage_Gabor: gt_image_GaborOrientation=%d" GT_NL, ret);
//    GtSpinLock_Unlock(&lock);
//    GtSpinLock_Unlock(&image->lock);
//
//    return ret;
//  }


  // Filter on each pixel
  for(y = nBorder; y < (image->height - nBorder); y++)
  {
    for(x = nBorder; x < (image->width - nBorder); x ++)
    {
      if(image->gabor_pp_b_buf[x][y] == 0)
      {
        image->gabor_pp_i_buf[x][y] = 1000;
        continue;
      }
      nPixel = 0;
      for(n = 0; n < image->gabor_filter_size; n++)
      {
        for(m = 0; m < image->gabor_filter_size; m ++)
        {
//          if(image->gabor_pp_i_data[x][y] < image->gabor_dir_no)
//          {
            if(image->gabor_coef[image->gabor_pp_i_data[x][y]][m][n] != 0.0)
            {
//              if(((m+x-nBorder) < image->width) && ((n+y-nBorder) < image->height))
                nPixel += (int)image->gabor_pp_b_data[m+x-nBorder][n+y-nBorder]*image->gabor_coef[image->gabor_pp_i_data[x][y]][m][n];
//              else
//                GT_LOGI("m+x-nBorder=%d, n+y-nBorder=%d" GT_NL, m+x-nBorder, n+y-nBorder);
            }
//          }
//          else
//          {
//            GT_LOGI("image->gabor_pp_i_data[x][y] > image->gabor_dir_no" GT_NL);
//          }
        }
      }
      image->gabor_pp_i_buf[x][y] = nPixel>>10;
    }
  }


	//
	for(y=0;y<image->height;y++) {
		for(x=0;x<image->width;x++) {
			image->gabor_pp_i_buf[x][y] = image->gabor_pp_i_buf[x][y]/2;
			if(image->gabor_pp_i_buf[x][y] > 255) {
				image->data[x + y * image->width] = 255;
			}
			else if(image->gabor_pp_i_buf[x][y] < 0) {
				image->data[x + y * image->width] = 0;
			}
			else {
				image->data[x + y * image->width] = (gt_byte)image->gabor_pp_i_buf[x][y];
			}
		}
	}


	// Clear border
	for(y = 0; y < image->height; y ++)
	{
		for(x = 0; x < nBorder; x ++)
		{
			image->data[x + y * image->width] = 255;
			image->data[(image->width - 1 - x) + y * image->width] = 255;
		}
	}

	for(y = 0; y < nBorder; y ++)
	{
		for(x = 0; x < image->width; x ++)
		{
			image->data[x + y * image->width] = 255;
			image->data[x + (image->height - 1 - y) * image->width] = 255;
		}
	}

//  GT_LOGI("GtImage_Gabor: ret=%d" GT_NL, ret);

  return ret;
}

int GtImage_GetDepthImage(GtImage_t *imageDepth, GtImage_t *imageLeft, GtImage_t *imageRight) {

		int x;
	int y;
	int i;
	int j;
	int width = imageDepth->width;
	int height = imageDepth->height;
	int *sum; 
	int depth;
	int width_2 = width - 2;
	int height_2 = height - 2;
	int shift;
	int min = 65535;
	int minx;
	int det;

	sum = gt_malloc(width * sizeof(int));

	if (imageDepth == NULL) return GT_ERROR_PARAMETER_0;
	if (imageDepth->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (imageLeft == NULL) return GT_ERROR_PARAMETER_1;
	if (imageLeft->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_1;
	if (imageRight == NULL) return GT_ERROR_PARAMETER_2;
	if (imageRight->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_2;

	if (imageDepth->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_0;
	if (imageLeft->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_1;
	if (imageRight->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_2;

	if (imageDepth->width != imageLeft->width) return GT_ERROR_PARAMETER_0;
	if (imageDepth->width != imageRight->width) return GT_ERROR_PARAMETER_0;
	if (imageDepth->height != imageLeft->height) return GT_ERROR_PARAMETER_0;
	if (imageDepth->height != imageRight->height) return GT_ERROR_PARAMETER_0;



	for (y = 0; y < height_2; ++y) {
		for (x = 0; x < width_2; ++x)	{
			sum[x] = 0;
			for (j = 0; j <= 2; ++j)	{
				for (i = 0; i <= 2; ++i)	{
					shift = x + i + width * (y + j);
					//GtImage_GetPixelColor(imageR, x+i, y+j, color);
					sum[x] += imageRight->data[shift];
				}
			}
		}

		for(x = 0; x < width_2; ++x) {
			int refgray = 0;
			for(j = 0; j <= 2; ++j) 	{
				for(i = 0; i <= 2; ++i) {
					shift = x + i + width * (y + j);
					//GtImage_GetPixelColor(imageL, x+i, y+j, color);
					refgray += imageLeft->data[shift];
				}
			}

			for (i = x; i < width_2; ++i) {
				det = refgray - sum[i];
				det = det * det;
				if (min > det) {
					min = det;
					minx = i;
				}
			}

			depth = 255 - (minx - x) * 10;

			shift = x + width * y;
			imageDepth->data[shift] = depth;
			//GtColor_SetGRAY8(color,depth);

			/*
			if(depth >= 0 && depth <= 63)
			{
				GtColor_SetRGB888(color,255,0,0);
			}
			else if (depth >= 64 && depth <= 127)
			{
				GtColor_SetRGB888(color,255,128,0);
			}
			else if (depth >= 128 && depth <= 191)
			{
				GtColor_SetRGB888(color,255,255,0);
			}
			else if (depth >= 192 && depth <= 256)
			{
				GtColor_SetRGB888(color,128,255,0);
			}

			*/


			/*if(depth <= 31)
			{
				GtColor_SetRGB888(color,255,0,0);
			}
			else if (depth >= 32 && depth <= 63)
			{
				GtColor_SetRGB888(color,255,128,0);
			}
			else if (depth >= 64 && depth <= 95)
			{
				GtColor_SetRGB888(color,255,255,0);
			}
			else if (depth >= 96 && depth <= 127)
			{
				GtColor_SetRGB888(color,128,255,0);
			}
			else if (depth >= 128 && depth <= 159)
			{
				GtColor_SetRGB888(color,0,255,0);
			}
			else if (depth >= 160 && depth <= 191)
			{
				GtColor_SetRGB888(color,0,255,128);
			}
			else if (depth >= 192 && depth <= 223)
			{
				GtColor_SetRGB888(color,0,255,255);
			}
			else if (depth >= 224&& depth <= 256)
			{
				GtColor_SetRGB888(color,0,0,255);
			}
			*/
			//GtImage_DrawPixel(imageDepth,x,y,color);
		}
	}
	gt_free(sum);

	return GT_OK;
}

int GtImage_Blob(GtImage_t *imageBlob, GtImage_t *imageSrc, int range) {

		int ret;
	int count;
	int i;
	int j;
	int x;
	int y;
	int width;
	int height;
	GtImageBlobPoint_t blobPoint;
	gt_int32  *dataBlob;
	gt_uint8  *dataGray8;
	GtArray_t *blobPoints;
	int diff;
	int gray8;

	if (imageBlob == NULL) return GT_ERROR_PARAMETER_0;
	if (imageBlob->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (imageBlob->colorMode != GT_COLOR_MODE_INT32) return GT_ERROR_PARAMETER_0;
	if (imageSrc == NULL) return GT_ERROR_PARAMETER_1;
	if (imageSrc->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_1;
	if (imageSrc->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_1;
	if (imageBlob->width != imageSrc->width) return GT_ERROR_PARAMETER_1;
	if (imageBlob->height != imageSrc->height) return GT_ERROR_PARAMETER_1;
	if (range <= 0) return GT_ERROR_PARAMETER_2;



	blobPoints = GtArray_New();
	if (blobPoints == NULL) {
		return GT_ERROR_NO_MEMORY;
	}
	ret = GtArray_Init(blobPoints, "GtImageBlobPoint_t", sizeof(GtImageBlobPoint_t), imageBlob->width * imageBlob->height);
	if (ret <= 0) {
		GtArray_Free(blobPoints);
		return ret;
	}

	count = 0;
	width = imageBlob->width;
	height= imageBlob->height;

	dataGray8 = (gt_uint8 *)imageSrc->data;
	dataBlob  = (gt_int32 *)imageBlob->data;

	memset(dataBlob, 0, imageBlob->dataSize);

	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			if(dataBlob[width*j+i] == 0) {
				count++;
				dataBlob[i + j * width] = count;
				blobPoint.x = i;
				blobPoint.y = j;
				blobPoint.gray8 = dataGray8[i + j * width];
				GtArray_Append(blobPoints, &blobPoint);

				while(GtArray_GetLength(blobPoints) > 0) {
					GtArray_Copy(blobPoints, 0, &blobPoint);
					x = blobPoint.x;
					y = blobPoint.y;
					gray8 = blobPoint.gray8;

					// Up
					if((y - 1) >= 0) {
						diff = dataGray8[width * (y - 1) + (x)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range)
						   &&(dataBlob[width * (y - 1) + (x)] == 0)) {
							dataBlob[width * (y - 1) + (x)] = count;
							blobPoint.x = (x);
							blobPoint.y = (y - 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Up - Right
					if((x + 1) < width && (y - 1) >= 0) {
						diff = dataGray8[width * (y - 1) + (x + 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y - 1) + (x + 1)] == 0)) {
							dataBlob[width * (y - 1) + (x + 1)] = count;
							blobPoint.x = (x + 1);
							blobPoint.y = (y - 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Right
					if((x + 1) < width) {
						diff = dataGray8[width * (y) + (x + 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y) + (x + 1)] == 0)){
							dataBlob[width * (y) + (x+1)] = count;
							blobPoint.x = (x + 1);
							blobPoint.y = (y);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Right - Down
					if((x + 1) < width && (y + 1) < height)
					{
						diff = dataGray8[width * (y + 1) + ( x + 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y + 1) + (x + 1)] == 0))
						{
							dataBlob[width * (y + 1) + ( x + 1)] = count;
							blobPoint.x = (x + 1);
							blobPoint.y = (y + 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Down
					if((y + 1) < height)
					{
						diff = dataGray8[width * (y + 1) + (x)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range)&&(dataBlob[width * (y + 1) + (x)]==0))
						{
							dataBlob[width * (y + 1) + (x)] = count;
							blobPoint.x = (x);
							blobPoint.y = (y + 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Down - Left
					if((x - 1) >= 0 && (y + 1) < height)
					{
						diff = dataGray8[width * (y + 1) + (x - 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y + 1) + (x - 1)] == 0))
						{
							dataBlob[width * (y + 1) + (x - 1)] = count;
							blobPoint.x = (x - 1);
							blobPoint.y = (y + 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Left
					if((x - 1) >= 0)
					{
						diff = dataGray8[width * (y) + (x - 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y) + (x - 1)] == 0))
						{
							dataBlob[width * (y) + (x - 1)] = count;
							blobPoint.x = (x - 1);
							blobPoint.y = (y);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}

					// Left - Up
					if((x - 1) >= 0 && (y - 1) >= 0)
					{
						diff = dataGray8[width * (y - 1) + (x - 1)] - gray8;
						if(diff < 0) {
							diff = (-1) * diff;
						}
						if((diff < range) && (dataBlob[width * (y - 1) + (x - 1)] == 0))
						{
							dataBlob[width * (y - 1) + (x - 1)] = count;
							blobPoint.x = (x - 1);
							blobPoint.y = (y - 1);
							GtArray_Append(blobPoints, &blobPoint);
						}
					}
					GtArray_Remove(blobPoints, 0);
				}
			}
		}
	}
	
	GtArray_Free(blobPoints);

	GT_LOGI("count = %d", count);

  return GT_OK;
}

int GtImage_ConvertBlobToGray8(GtImage_t *imageDst, GtImage_t *imageBlob) {
	
	int x;
	int y;
	int shift;
	int width = imageDst->width;
	int height = imageDst->height;
	gt_uint8 *dataGray8 = (gt_uint8 *)imageDst->data;
	gt_int32 *dataInt32 = (gt_int32 *)imageBlob->data;

	if (imageDst == NULL) return GT_ERROR_PARAMETER_0;
	if (imageDst->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (imageDst->colorMode != GT_COLOR_MODE_GRAY8) return GT_ERROR_PARAMETER_0;
	if (imageBlob == NULL) return GT_ERROR_PARAMETER_1;
	if (imageBlob->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_1;
	if (imageBlob->colorMode != GT_COLOR_MODE_INT32) return GT_ERROR_PARAMETER_1;
	if (imageDst->width != imageBlob->width) return GT_ERROR_PARAMETER_1;
	if (imageDst->height != imageBlob->height) return GT_ERROR_PARAMETER_1;



	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			shift = x + y * width;
			dataGray8[shift] = (dataInt32[shift] % 256);
		}
	}

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
