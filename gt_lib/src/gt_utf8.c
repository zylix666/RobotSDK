/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_utf8.h"

#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#if defined(GT_CONFIG_WINDOWS)
  #include <windows.h>
  #include <locale.h>
#elif defined(GT_CONFIG_LINUX) 

#endif 

#ifdef __cplusplus
extern "C" 
{
#endif

static const gt_utf8 gt_utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

const gt_utf8 *gtUtf8Skip = gt_utf8_skip_data;

#define gtUtf8_NextChar(p) (gt_utf8 *)((p) + gtUtf8Skip[*(const gt_uchar *)(p)])

static int gtUtf8IsInit = 0;

int GtUtf8_InitLib() {
	if (gtUtf8IsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtUtf8IsInit = 1;
	
	return 1;
}

gt_utf8 *GtUtf8_NextChar(const gt_utf8 *str) {
  if (str == NULL) return NULL;
    
  return  (gt_utf8 *)gtUtf8_NextChar(str);
}

int GtUtf8_GetLength(const gt_utf8 *str) {
  gt_int len;
//  const gt_utf8 *start;
  gt_utf8 *str_ptr;

  if (str == NULL) return 0;  
  if (str[0] == '\0') return 0; 
  
  str_ptr = (gt_utf8 *)str;
  
  len = 0;
//  start = str;        

  while (*str_ptr) { 
    len++;
       
    str_ptr = gtUtf8_NextChar(str_ptr);            
  }

  return len;
}

int GtUtf8_GetSize(const gt_utf8 *str) {
  gt_int size;
  const gt_utf8 *start;
  gt_utf8 *str_ptr;
  
  if (str == NULL) return 0;
  if (str[0] == '\0') return 0;
  
  size = 0;
  start = str;
  str_ptr = (gt_utf8 *)str;        

  while (*str_ptr) {
    str_ptr = gtUtf8_NextChar(str_ptr);          
  }
  
  size = (int)(str_ptr - start);
  
  return size;  
}

int GtUtf8_ToWchar(wchar_t *wstr, gt_int wstr_length, const gt_utf8 *str) {
  int len;
  if (wstr == NULL) return GT_ERROR_PARAMETER_0;
  if (wstr_length <= 0) return GT_ERROR_PARAMETER_1;
    
  wstr[0] = 0;
  if (str == NULL) return GT_ERROR_PARAMETER_2;
    
  if (GtUtf8_GetLength(str) == 0) {
    return GT_ERROR_PARAMETER_2;    
  }

#if defined(GT_CONFIG_VC)
  len = MultiByteToWideChar(CP_UTF8, 
                            0,         
                            str, 
                            -1,  // Byte length.       
                            wstr,  
                            wstr_length * sizeof(wchar_t));  
#else
  len = mbstowcs(wstr, (const char*)str, wstr_length);
#endif   

  return len;                        
}

int GtUtf8_FromWchar(gt_utf8 *str, gt_int str_size, const wchar_t *wstr) {
  if (str == NULL) return GT_ERROR_PARAMETER_0;
  if (str_size <= 0) return GT_ERROR_PARAMETER_1;
  str[0] = 0;
  if (wstr == NULL) return GT_ERROR_PARAMETER_2;

  if (wcslen(wstr) == 0) {
    return GT_ERROR_PARAMETER_2;    
  }
  
#if defined(GT_CONFIG_VC)   
  WideCharToMultiByte(CP_ACP, 
                      0, 
                      wstr,
                      (gt_int)wcslen(wstr), 
                      str, 
                      str_size,
                      NULL,    
                      NULL);  
#else
  wcsrtombs((char*)str, &wstr, str_size, NULL);
#endif                      
  
  return GT_OK;                        
}

int GtUtf8_ToLowerCase(gt_utf8 *str) {
  if (str == NULL) return GT_ERROR_NULL_POINTER;

  while (*str) {
    switch(*str) {
      case 'A': *str = 'a'; break;
      case 'B': *str = 'b'; break;
      case 'C': *str = 'c'; break;
      case 'D': *str = 'd'; break;
      case 'E': *str = 'e'; break;
      case 'F': *str = 'f'; break;
      case 'G': *str = 'g'; break;
      case 'H': *str = 'h'; break;
      case 'I': *str = 'i'; break;
      case 'J': *str = 'j'; break;
      case 'K': *str = 'k'; break;
      case 'L': *str = 'l'; break;
      case 'M': *str = 'm'; break;
      case 'N': *str = 'n'; break;
      case 'O': *str = 'o'; break;
      case 'P': *str = 'p'; break;
      case 'Q': *str = 'q'; break;
      case 'R': *str = 'r'; break;
      case 'S': *str = 's'; break;
      case 'T': *str = 't'; break;
      case 'U': *str = 'u'; break;
      case 'V': *str = 'v'; break;
      case 'W': *str = 'w'; break;
      case 'X': *str = 'x'; break;
      case 'Y': *str = 'y'; break;
      case 'Z': *str = 'z'; break;                                                                          
    }
    
    str = gtUtf8_NextChar(str);
  }  

  return GT_OK;  
}

int GtUtf8_ToUpperCase(gt_utf8 *str) {
  if (str == NULL) return GT_ERROR_NULL_POINTER;

  while (*str) {
    switch(*str) {
      case 'a': *str = 'A'; break;
      case 'b': *str = 'B'; break;
      case 'c': *str = 'C'; break;
      case 'd': *str = 'D'; break;
      case 'e': *str = 'E'; break;
      case 'f': *str = 'F'; break;
      case 'g': *str = 'G'; break;
      case 'h': *str = 'H'; break;
      case 'i': *str = 'I'; break;
      case 'j': *str = 'J'; break;
      case 'k': *str = 'K'; break;
      case 'l': *str = 'L'; break;
      case 'm': *str = 'M'; break;
      case 'n': *str = 'N'; break;
      case 'o': *str = 'O'; break;
      case 'p': *str = 'P'; break;
      case 'q': *str = 'Q'; break;
      case 'r': *str = 'R'; break;
      case 's': *str = 'S'; break;
      case 't': *str = 'T'; break;
      case 'u': *str = 'U'; break;
      case 'v': *str = 'V'; break;
      case 'w': *str = 'W'; break;
      case 'x': *str = 'X'; break;
      case 'y': *str = 'Y'; break;
      case 'z': *str = 'Z'; break;                                                                          
    }
    
    str = gtUtf8_NextChar(str);
  }
  return GT_OK;  
}

int GtUtf8_GetIndexShift(const gt_utf8 *str, int index) { 
  gt_int shift;
  gt_int count;
  gt_int str_length;
  gt_utf8 *str_ptr;
  const gt_utf8 *start;
  
  if (str == NULL) return GT_ERROR_PARAMETER_0;
  if (index < 0) return GT_ERROR_PARAMETER_1;

  if (index == 0) return 0;
  
  str_length = GtUtf8_GetLength(str);
  if (index >= str_length) {
    return GT_ERROR_PARAMETER_1;
  }
  
  shift = 0;
  count = 0;  
  str_ptr = (gt_utf8 *)str;
  start = str;  

  while (*str_ptr) {
    str_ptr = gtUtf8_NextChar(str_ptr);
    count ++;
    
    if (count >= index)
      break;
  }
  
  shift = (int)(str_ptr - start);

  return shift; 
}

int GtUtf8_GetIndex(const gt_utf8 *str, const gt_utf8 *str_find) {
  gt_int index;
  gt_int search_length;
  gt_int  str_size;
  gt_int  str_length;
  gt_int  str_find_size;  
  gt_int  str_find_length;
  gt_utf8 *str_ptr_from;  
  
  if (str == NULL) return GT_ERROR_PARAMETER_0;
  if (str_find == NULL) return GT_ERROR_PARAMETER_1;

  str_size      = GtUtf8_GetSize(str);
  str_find_size = GtUtf8_GetSize(str_find);
  
  if (str_find_size > str_size) {
    return GT_ERROR_PARAMETER_1;
  }
  
  str_length = GtUtf8_GetLength(str);
  str_find_length = GtUtf8_GetLength(str_find);
  
  if (str_find_length > str_length) {
    return GT_ERROR_PARAMETER_1;
  }
  
  index = 0;
  str_ptr_from = (gt_utf8 *)str;
  search_length = str_length - str_find_length;
  
  while (index <= search_length) {        
    if (gt_memcmp(str_ptr_from, str_find, str_find_size) == 0) {
      break;
    }

    str_ptr_from = gtUtf8_NextChar(str_ptr_from);    
    index ++;
  }   
  
	// Not found.
	if (index > search_length) {
		index = GT_ERROR_NOT_FOUND;
	}
  
  return index;
}

int GtUtf8_GetLastIndex(const gt_utf8 *str, const gt_utf8 *str_find) {
  gt_int index;
  gt_int last_index;
  gt_utf8 *str_ptr;
  
  if (str == NULL) return GT_ERROR_PARAMETER_0;
  if (str_find == NULL) return GT_ERROR_PARAMETER_1;
  
  index = GtUtf8_GetIndex(str, str_find);
  if (index < 0) {
    return index;
  }
  
  while (index >= 0) {
    last_index = index;
    str_ptr = (gt_utf8 *)(str + GtUtf8_GetIndexShift(str, index));
    str_ptr = gtUtf8_NextChar(str_ptr);
    index = GtUtf8_GetIndex(str_ptr, str_find);
  }
  
  last_index = GtUtf8_GetLength(str) - last_index - 1;
 
  return last_index;
}

int GtUtf8_GetChar(const gt_utf8 *str, gt_int index, gt_utf8 *char_buf, gt_int char_buf_size) {
  gt_utf8 *this_char;
  gt_utf8 *next_char;
  gt_long  this_char_size;
  gt_int  i;
  
  if (char_buf == NULL)    return GT_ERROR_PARAMETER_2;
  if (char_buf_size == 0)  return GT_ERROR_PARAMETER_3;
  
  gt_memset(char_buf, 0, char_buf_size);  // Cannot use char_buf[0] = 0; !!!
  
  if (str == NULL)  return GT_ERROR_PARAMETER_0;
  
  if (GtUtf8_GetLength(str) <= index)  return GT_ERROR_PARAMETER_1;
    
  this_char = (gt_utf8 *)str;
  
  for (i = 0; i < index; i ++) {
    this_char = gtUtf8_NextChar(this_char);
  }
  
  next_char = gtUtf8_NextChar(this_char);
  
  if (this_char == NULL || next_char == NULL) {
    return GT_ERROR; 
  }
  
  this_char_size = (gt_long)next_char - (gt_long)this_char;
  
  if (this_char_size <= 0) {
    return GT_ERROR;
  }
  
  gt_memcpy(char_buf, this_char, this_char_size);
    
  return GT_OK;
}

int GtUtf8_SubString(gt_utf8 *str_to, gt_int str_to_size, const gt_utf8 *str_from, gt_int from, gt_int to) {
  gt_int  str_from_len;
  gt_int shift_from;
  gt_int shift_to;
  gt_int copy_size;
  
  if (str_to == NULL) return GT_ERROR_PARAMETER_0;
  if (str_to_size == 0) return GT_ERROR_PARAMETER_1;

  str_to[0] = 0;
  if (str_from == NULL) return GT_ERROR_PARAMETER_2;
  if (from > to) return GT_ERROR_PARAMETER_3;  
  str_from_len = GtUtf8_GetLength(str_from);
  if (str_from_len == 0) return GT_ERROR_PARAMETER_2;
  if (to > str_from_len) return GT_ERROR_PARAMETER_4;  
  
  shift_from = GtUtf8_GetIndexShift(str_from, from);
  if (shift_from < 0) {
    return GT_ERROR; 
  }
  
  shift_to = GtUtf8_GetIndexShift(str_from, to);
  if (shift_to < 0) {
    return GT_ERROR_PARAMETER_4; 
  }
  
  copy_size = shift_to - shift_from;
  if (copy_size < 0 || ((copy_size + 1) > str_to_size)) {
    return GT_ERROR_PARAMETER_4; 
  }
  
  gt_memcpy(str_to, str_from + shift_from, copy_size);
  str_to[copy_size] = 0;

  return GT_OK;
}

gt_utf8 *GtUtf8_GetCharAddress(const gt_utf8 *str, gt_int index) {
  gt_utf8 *thisChar;
  gt_int  i;

  if (str == NULL)  return NULL;
  
  if (GtUtf8_GetLength(str) <= index)  return NULL;
    
  thisChar = (gt_utf8 *)str;
  
  for (i = 0; i < index; i ++) {
    thisChar = gtUtf8_NextChar(thisChar);
  }
  
  return thisChar;
}

#ifdef __cplusplus
}
#endif
