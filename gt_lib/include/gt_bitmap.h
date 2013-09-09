/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_BITMAP_H__
#define __GT_BITMAP_H__

#include "gt_lib.h"
#include "gt_image.h"

#define GT_BITMAP_HEADER         "BM"
#define GT_BITMAP_HEADER_LENGTH  2   
  
#ifdef GT_CONFIG_VC
  #pragma pack(push)
  #pragma pack(1)
#else
  #pragma pack(1)
#endif

typedef struct tagGT_BITMAPFILEHEADER { 
	gt_uint16 bfType;
	gt_uint32 bfSize;
	gt_uint16 bfReserved1;
	gt_uint16 bfReserved2;
	gt_uint32 bfOffBits;
} GT_BITMAPFILEHEADER;  

#ifdef GT_CONFIG_VC
  #pragma pack(pop)
#else
  #pragma pack()
#endif
   
typedef struct tagGT_BITMAPINFOHEADER { 
	gt_uint32 biSize;
	gt_int32  biWidth;
	gt_int32  biHeight;
	gt_uint16 biPlanes;
	gt_uint16 biBitCount;
	gt_uint32 biCompression;
	gt_uint32 biSizeImage;
	gt_int32  biXPelsPerMeter;
	gt_int32  biYPelsPerMeter;
	gt_uint32 biClrUsed;
	gt_uint32 biClrImportant;
} GT_BITMAPINFOHEADER;
    
typedef struct tagGTRGBQUAD {
	gt_byte rgbBlue;
	gt_byte rgbGreen;
	gt_byte rgbRed;
	gt_byte rgbReserved;
} GT_RGBQUAD;  
   
typedef struct tagGT_BITMAPINFO {
	GT_BITMAPINFOHEADER bmiHeader;
	GT_RGBQUAD bmiColors[1];
} GT_BITMAPINFO;

#ifdef __cplusplus
extern "C"
{
#endif

int GtBitmap_InitLib();

int GtBitmap_Load(GtImage_t *image, gt_byte *bitmapBuffer, int bitmapBufferLength);

int GtBitmap_LoadFile(GtImage_t *image, gt_utf8 *path);

int GtBitmap_Save(GtImage_t *image, gt_byte **bitmapBuffer);

int GtBitmap_SaveFile(GtImage_t *image, gt_utf8 *path);

#ifdef __cplusplus
}
#endif

#endif /* __GT_BITMAP_H__ */
