/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_UTF8_H__
#define __GT_UTF8_H__

#include "gt_lib.h"

#define GT_WSTR_LENGTH (GT_STR_SIZE/2)
#define GT_CHAR_SIZE 5  // 4 bytes for UTF-8. 1 byte for ending.

#ifdef __cplusplus
extern "C" 
{
#endif

gt_utf8 *GtUtf8_NextChar(const gt_utf8 *str);

int GtUtf8_GetLength(const gt_utf8 *str);

int GtUtf8_GetSize(const gt_utf8 *str); // The size and len is only matched in ASCII code.

int GtUtf8_ToWchar(wchar_t *wstr, gt_int wstr_length, const gt_utf8 *str);

int GtUtf8_FromWchar(gt_utf8 *str, gt_int str_size, const wchar_t *wstr);

int GtUtf8_ToLowerCase(gt_utf8 *str);

int GtUtf8_ToUpperCase(gt_utf8 *str);

int GtUtf8_GetIndexShift(const gt_utf8 *str, gt_int index);

int GtUtf8_GetIndex(const gt_utf8 *str, const gt_utf8 *str_find);

int GtUtf8_GetLastIndex(const gt_utf8 *str, const gt_utf8 *str_find);

int GtUtf8_SubString(gt_utf8 *str_to, gt_int str_to_size, const gt_utf8 *str_from, gt_int from, gt_int to);

int GtUtf8_GetChar(const gt_utf8 *str, gt_int index, gt_utf8 *char_buf, gt_int char_buf_size);

gt_utf8 *GtUtf8_GetCharAddress(const gt_utf8 *str, gt_int index);

#ifdef __cplusplus
}
#endif

#endif /* __GT_UTF8_H__ */
