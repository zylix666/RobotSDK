/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_bitmap.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtBitmapIsInit = 0;

int GtBitmap_InitLib() {
	if (gtBitmapIsInit == 1) return GT_OK_INIT_ALREADY;

	gtBitmapIsInit = 1;
	
    return GT_OK;
}

int GtBitmap_Load(GtImage_t *image, gt_byte *bitmapBuffer, int bitmapBufferLength) {

	
	GT_BITMAPFILEHEADER *bmfh;
  	GT_BITMAPINFOHEADER *bmih;
	 GT_COLOR_MODE_t colorMode = GT_COLOR_MODE_UNKNOWN; 
	int row_align;
  	int i;
    int j;
    int image_shift;
  	int buf_shift;
  	int dataSize;
	gt_byte *data;

	if (gtBitmapIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (bitmapBuffer == NULL) return GT_ERROR_PARAMETER_1;
	if (bitmapBufferLength <= 0) return GT_ERROR_PARAMETER_2;


  	bmfh = (GT_BITMAPFILEHEADER *)bitmapBuffer;
  	bmih = (GT_BITMAPINFOHEADER *)(bitmapBuffer + sizeof(GT_BITMAPFILEHEADER));



  	if (bmih->biBitCount == 24) {
  		colorMode = GT_COLOR_MODE_RGB888;
  	} else if (bmih->biBitCount == 8) {
  		colorMode = GT_COLOR_MODE_GRAY8;
  	} else {
  		return GT_ERROR_COLOR_MODE;
  	}

  	GtImage_InitImage(image, bmih->biWidth, bmih->biHeight, colorMode);

	data = bitmapBuffer + bmfh->bfOffBits;

  	if (colorMode == GT_COLOR_MODE_RGB888) {
		row_align = (bmih->biWidth * 3) % 4;
	 	row_align = 4 - row_align;
		if (row_align == 4) row_align = 0;

		dataSize = image->dataSize;
		for (j = 0; j < image->height; j++) {
			for (i = 0; i < image->width; i++) {
				image_shift = (i + j * image->width) * 3;
				buf_shift = 3 * i + (bmih->biHeight - 1 - j) * (bmih->biWidth * 3 + row_align);
				if ((buf_shift + 2) < dataSize) {
					image->data[image_shift  ]   = data[buf_shift + 2];
					image->data[image_shift+1]   = data[buf_shift + 1];
					image->data[image_shift+2]   = data[buf_shift  ];
				}
			}
		}
	} else if (colorMode == GT_COLOR_MODE_GRAY8) {
		row_align = bmih->biWidth % 4;
	 	row_align = 4 - row_align;
		if (row_align == 4) row_align = 0;

		dataSize = image->dataSize;
		for (j = 0; j < image->height; j++) {
			for (i = 0; i < image->width; i++) {
				image_shift = i + j * image->width;
				//buf_shift = image_shift;
				buf_shift = i + (bmih->biHeight - 1 - j) * (bmih->biWidth + row_align);
				if (buf_shift < dataSize) {
					image->data[image_shift]   = data[buf_shift];
				}
			}
		}
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	return GT_OK;
}

int GtBitmap_LoadFile(GtImage_t *image, gt_utf8 *path) {

	GtFile_t *gtFile;
	gt_byte *fileData;
	gt_int64 fileSize;
	int ret ;
	if (gtBitmapIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (path == NULL) return GT_ERROR_PARAMETER_1;

	gtFile = GtFile_New();
	if (gtFile == NULL) {
		return GT_ERROR_NEW_OBJECT;
	}

	ret = GtFile_Open(gtFile, path, GT_FILE_O_RDONLY);
	if (ret <= 0) {
		return ret;
	}

	fileSize = GtFile_GetSize(gtFile);
	if (fileSize < 0) {
		return (int)fileSize;
	}

	fileData = gt_calloc(1, (int)fileSize);
	if (fileData == NULL) {
		return GT_ERROR_NO_MEMORY;
	}

	ret = (int)GtFile_Read(gtFile, fileData, (int)fileSize);
	if (ret <= 0) {
		return ret;
	}

	ret = (int)GtBitmap_Load(image, fileData, (int)fileSize);
	if (ret <= 0) {
		return ret;
	}

	GtFile_Close(gtFile);

	return GT_OK;
}

int GtBitmap_Save(GtImage_t *image, gt_byte **bitmapBuffer) {
	
	gt_byte *buf;
	gt_byte *data;
	int i;
	int j;
	int image_shift;
	int buf_shift;
	int row_align;
	GT_BITMAPFILEHEADER *bmfh;
	GT_BITMAPINFOHEADER *bmih;
	int buf_len = 0;
	gt_byte *palette;
	int palette_shift;
	
	
	if (gtBitmapIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (bitmapBuffer == NULL) return GT_ERROR_PARAMETER_1;



	if (image->colorMode == GT_COLOR_MODE_RGB888) {
		row_align = (image->width * 3) % 4;
		row_align = 4 - row_align;
		if (row_align == 4) row_align = 0;

		buf_len = sizeof(GT_BITMAPFILEHEADER) + sizeof(GT_BITMAPINFOHEADER) + (image->width * 3 + row_align) * image->height;

		buf = (gt_byte *)gt_calloc(1, buf_len);

		bmfh = (GT_BITMAPFILEHEADER *)buf;
		bmih = (GT_BITMAPINFOHEADER *)(buf + sizeof(GT_BITMAPFILEHEADER));

		// File.
		bmfh->bfType = 0x4D42;
		bmfh->bfSize = buf_len;
		bmfh->bfReserved1 = 0;
		bmfh->bfReserved2 = 0;
		bmfh->bfOffBits = sizeof(GT_BITMAPFILEHEADER) + sizeof(GT_BITMAPINFOHEADER);

		// Info.
		bmih->biSize = sizeof(GT_BITMAPINFOHEADER);
		bmih->biWidth = image->width;
		bmih->biHeight = image->height;
		bmih->biPlanes = 1;
		bmih->biBitCount = 24;
		bmih->biCompression = 0;
		bmih->biSizeImage = buf_len - bmfh->bfOffBits;
		bmih->biXPelsPerMeter = 2835;
		bmih->biYPelsPerMeter = 2835;
		bmih->biClrUsed = 0;
		bmih->biClrImportant = 0;

		// Data.
		data = buf+bmfh->bfOffBits;
		for (j = 0; j < image->height; j++) {
     		for (i = 0; i < image->width; i++) {
				image_shift = (i + j * image->width) * 3;
				buf_shift = 3 * i + (bmih->biHeight - 1 - j) * (bmih->biWidth * 3 + row_align);
				data[buf_shift+2] = image->data[image_shift  ];
				data[buf_shift+1] = image->data[image_shift+1];
				data[buf_shift  ] = image->data[image_shift+2];
			}
		}

		*bitmapBuffer = buf;
	} else if (image->colorMode == GT_COLOR_MODE_GRAY8) {
		row_align = (image->width) % 4;
		row_align = 4 - row_align;
		if (row_align == 4) row_align = 0;

		buf_len = sizeof(GT_BITMAPFILEHEADER) + sizeof(GT_BITMAPINFOHEADER) + 1024 + (image->width + row_align) * image->height;

		buf = (gt_byte *)gt_calloc(1, buf_len);

		bmfh = (GT_BITMAPFILEHEADER *)buf;
		bmih = (GT_BITMAPINFOHEADER *)(buf + sizeof(GT_BITMAPFILEHEADER));

		// File.
		bmfh->bfType = 0x4D42;
		bmfh->bfSize = buf_len;
		bmfh->bfReserved1 = 0;
		bmfh->bfReserved2 = 0;
		bmfh->bfOffBits = sizeof(GT_BITMAPFILEHEADER) + sizeof(GT_BITMAPINFOHEADER);

		// Info.
		bmih->biSize = sizeof(GT_BITMAPINFOHEADER);
		bmih->biWidth = image->width;
		bmih->biHeight = image->height;
		bmih->biPlanes = 1;
		bmih->biBitCount = 8;
		bmih->biCompression = 0;
		bmih->biSizeImage = buf_len - bmfh->bfOffBits;
		bmih->biXPelsPerMeter = 2835;
		bmih->biYPelsPerMeter = 2835;
		bmih->biClrUsed = 0;
		bmih->biClrImportant = 0;

		// Palette 256 x 4
		palette = buf + bmfh->bfOffBits;

		for (i = 0; i < 256; i ++) {
			palette_shift = i * 4;
			palette[palette_shift] = i;
			palette[palette_shift + 1] = i;
			palette[palette_shift + 2] = i;
			palette[palette_shift + 3] = 0;
		}

		// Data.
		data = buf + bmfh->bfOffBits + 1024;
		for (j = 0; j < image->height; j++) {
     		for (i = 0; i < image->width; i++) {
				image_shift = i + j * image->width;
				//buf_shift = image_shift;
				buf_shift = i + (bmih->biHeight - 1 - j) * (bmih->biWidth + row_align);
				data[buf_shift] = image->data[image_shift];
			}
		}

		*bitmapBuffer = buf;
		//GT_LOGI("here, buf_len = %d", buf_len);
	} else {
		return GT_ERROR_COLOR_MODE;
	}

	return buf_len;
}

int GtBitmap_SaveFile(GtImage_t *image, gt_utf8 *path) {
	int ret;
	GtFile_t *gtFile;
	gt_byte *fileData;
	int fileSize;
	if (gtBitmapIsInit == 0) return GT_ERROR_NOT_INIT;
	if (image == NULL) return GT_ERROR_PARAMETER_0;
	if (image->magic != GT_MAGIC_IMAGE) return GT_ERROR_PARAMETER_0;
	if (path == NULL) return GT_ERROR_PARAMETER_1;

	gtFile = GtFile_New();
	if (gtFile == NULL) {
		return GT_ERROR_NEW_OBJECT;
	}

	ret = GtFile_Open(gtFile, path, GT_FILE_O_WRONLY | GT_FILE_O_CREAT | GT_FILE_O_TRUNC);
	if (ret <= 0) {
		return ret;
	}

	
	fileSize = GtBitmap_Save(image, &fileData);
	if (fileSize <= 0) {
		return fileSize;
	}

	ret = (int)GtFile_Write(gtFile, fileData, (int)fileSize);
	if (ret <= 0) {
		return ret;
	}

	ret = GtFile_Sync(gtFile);
	if (ret <= 0) {
		return ret;
	}

	GtFile_Close(gtFile);

	return GT_OK;
}

#ifdef __cplusplus
}
#endif
