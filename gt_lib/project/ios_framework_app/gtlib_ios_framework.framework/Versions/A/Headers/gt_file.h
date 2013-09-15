/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_FILE_H__
#define __GT_FILE_H__

#include "gt_lib.h"

#if defined(GT_CONFIG_VC)
	#define GT_FILE_O_RDONLY       _O_RDONLY
	#define GT_FILE_O_WRONLY       _O_WRONLY
	#define GT_FILE_O_RDWR         _O_RDWR
	#define GT_FILE_O_CREAT        _O_CREAT
	#define GT_FILE_O_EXCL         _O_EXCL
//  #define GT_FILE_O_NOCTTY       _O_NOCTTY
	#define GT_FILE_O_TRUNC        _O_TRUNC  
	#define GT_FILE_O_APPEND       _O_APPEND
//  #define GT_FILE_O_NONBLOCK     _O_NONBLOCK
//  #define GT_FILE_O_NDELAY     _O_NDELAY
//  #define GT_FILE_O_SYNC       _O_SYNC
//  #define GT_FILE_O_NOFOLLOW       _O_NOFOLLOW
  
	#define GT_FILE_O_BINARY       _O_BINARY
	#define GT_FILE_O_TEXT         _O_TEXT
	#define GT_FILE_O_NOINHERIT    _O_NOINHERIT
	#define GT_FILE_O_RANDOM       _O_RANDOM    
	#define GT_FILE_O_SEQUENTIAL   _O_SEQUENTIAL
	#define GT_FILE_O_SHORT_LIVED  _O_SHORT_LIVED
	#define GT_FILE_O_U16TEXT      _O_U16TEXT
	#define GT_FILE_O_U8TEXT       _O_U8TEXT
	#define GT_FILE_O_WTEXT        _O_WTEXT

	#define GT_FILE_S_IREAD        _S_IREAD
	#define GT_FILE_S_IWRITE       _S_IWRITE
#else  
	#define GT_FILE_O_RDONLY       O_RDONLY
	#define GT_FILE_O_WRONLY       O_WRONLY
	#define GT_FILE_O_RDWR         O_RDWR
	#define GT_FILE_O_CREAT        O_CREAT
	#define GT_FILE_O_EXCL         O_EXCL
	#define GT_FILE_O_NOCTTY       O_NOCTTY
	#define GT_FILE_O_TRUNC        O_TRUNC  
	#define GT_FILE_O_APPEND       O_APPEND
	#define GT_FILE_O_NONBLOCK     O_NONBLOCK
	#define GT_FILE_O_NDELAY       O_NDELAY
	#define GT_FILE_O_SYNC         O_SYNC
	#define GT_FILE_O_NOFOLLOW     O_NOFOLLOW
  
//  #define GT_FILE_O_BINARY       O_BINARY
//  #define GT_FILE_O_TEXT         O_TEXT
//  #define GT_FILE_O_NOINHERIT    O_NOINHERIT
//  #define GT_FILE_O_RANDOM       O_RANDOM    
//  #define GT_FILE_O_SEQUENTIAL   O_SEQUENTIAL
//  #define GT_FILE_O_SHORT_LIVED  O_SHORT_LIVED
//  #define GT_FILE_O_U16TEXT      O_U16TEXT
//  #define GT_FILE_O_U8TEXT       O_U8TEXT
//  #define GT_FILE_O_WTEXT        O_WTEXT

//  #define GT_FILE_S_IREAD        S_IREAD
//  #define GT_FILE_S_IWRITE       S_IWRITE

	#define GT_FILE_S_IRWXU        S_IRWXU
	#define GT_FILE_S_IRUSR        S_IRUSR
	#define GT_FILE_S_IWUSR        S_IWUSR
	#define GT_FILE_S_IXUSR        S_IXUSR
	#define GT_FILE_S_IRWXG        S_IRWXG
	#define GT_FILE_S_IRGRP        S_IRGRP
	#define GT_FILE_S_IWGRP        S_IWGRP
	#define GT_FILE_S_IXGRP        S_IXGRP
	#define GT_FILE_S_IRWXO        S_IRWXO
	#define GT_FILE_S_IROTH        S_IROTH
	#define GT_FILE_S_IWOTH        S_IWOTH
	#define GT_FILE_S_IXOTH        S_IXOTH
#endif

#define GT_FILE_SEEK_SET   SEEK_SET
#define GT_FILE_SEEK_CUR   SEEK_CUR
#define GT_FILE_SEEK_END   SEEK_END

typedef struct {
#if defined (GT_CONFIG_VC)
	HANDLE sys_file;
#else
	int sys_file;
#endif

	void       *lock;
	GT_MAGIC_t  magic;
} GtFile_t;  

#ifdef __cplusplus
extern "C"
{
#endif

int GtFile_InitLib();

GtFile_t *GtFile_New();

int GtFile_Free(GtFile_t *gtFile);

int GtFile_Lock(GtFile_t *gtFile);

int GtFile_Unlock(GtFile_t *gtFile);

int GtFile_Open(GtFile_t *gtFile, gt_utf8 *path , int flags);

int GtFile_Close(GtFile_t *gtFile);

gt_int64 GtFile_Write(GtFile_t *gtFile, const void *buffer, gt_int64 count);

gt_int64 GtFile_Read(GtFile_t *gtFile, void *buffer, gt_int64 count);

gt_int64 GtFile_Seek(GtFile_t *gtFile, gt_int64 offset, int origin);

int GtFile_Sync(GtFile_t *gtFile);

gt_int64 GtFile_GetSize(GtFile_t *gtFile);

int GtFile_Chmod(gt_utf8 *path, int mode);

#ifdef __cplusplus
}
#endif
   
#endif /* __GT_FILE_H__ */
