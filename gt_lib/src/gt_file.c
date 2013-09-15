/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_file.h"

#ifdef __cplusplus
extern "C" 
{
#endif

static int gtFileIsInit = 0;

int GtFile_InitLib() {
	if (gtFileIsInit == 1) {
		return GT_OK_INIT_ALREADY;
	}
	gtFileIsInit = 1;
	
    return GT_OK;
}

#if !defined (GT_CONFIG_VC)

GtFile_t *GtFile_New() {
	
	GtFile_t *gtFile;
	if (gtFileIsInit == 0) return NULL;

	gtFile = (GtFile_t *)gt_calloc(1, sizeof(GtFile_t));
	if (gtFile == NULL) {
		return NULL;
	}

	gtFile->sys_file = -1;

	gtFile->magic = GT_MAGIC_FILE;

	return gtFile;
}

int GtFile_Free(GtFile_t *gtFile) {
	if (gtFile == NULL) return GT_ERROR_NULL_POINTER;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	gtFile->magic = GT_MAGIC_UNKNOWN;

	gt_free(gtFile);

	return GT_OK;
}

int GtFile_Lock(GtFile_t *gtFile) {
	if (gtFile == NULL) return GT_ERROR_NULL_POINTER;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	if (gtFile->lock == NULL) {
		gtFile->lock  = GtLock_New();
		if (gtFile->lock == NULL) {
			return GT_ERROR_NEW_LOCK;
		}
	}

	return GtLock_Lock(gtFile->lock);
}

int GtFile_Unlock(GtFile_t *gtFile) {
	if (gtFile== NULL) return GT_ERROR_NULL_POINTER;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_MAGIC;

	return GtLock_Unlock(gtFile->lock);
}

int GtFile_Open(GtFile_t *gtFile, gt_utf8 *path , int flags) {
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;
	if (path == NULL) return GT_ERROR_PARAMETER_1;

	gtFile->sys_file = open((const char *)path, flags);
	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	return GT_OK;
}

int GtFile_Close(GtFile_t *gtFile) {
	int ret;
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;

	if (gtFile->sys_file < 0) {
		return GT_OK_DONE_ALREADY;
	}

	ret = close(gtFile->sys_file);
  
  	if (ret != 0) {
    	return GT_ERROR;
    }

	return GT_OK;
}

gt_int64 GtFile_Read(GtFile_t *gtFile, void *buffer, gt_int64 count) {
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;
	if (buffer == NULL) return GT_ERROR_PARAMETER_1;
	if (count <= 0) return GT_ERROR_PARAMETER_2;

	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	return read(gtFile->sys_file, buffer, count);
}

gt_int64 GtFile_Write(GtFile_t *gtFile, const void *buffer, gt_int64 count) {
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;
	if (buffer == NULL) return GT_ERROR_PARAMETER_1;
	if (count <= 0) return GT_ERROR_PARAMETER_2;

	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	return write(gtFile->sys_file, buffer, count);
}

gt_int64 GtFile_Seek(GtFile_t *gtFile, gt_int64 offset, int origin) {
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;

	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	return lseek(gtFile->sys_file, offset, origin);  // lseek = lseek64 in SH4 tool chain.
}

int GtFile_Sync(GtFile_t *gtFile) {
	int ret;
	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;

	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	ret = fsync(gtFile->sys_file);
  
  	if (ret != 0) {
  		return GT_ERROR;	
  	}
    
	return GT_OK;
}

gt_int64 GtFile_GetSize(GtFile_t *gtFile) {

	gt_int64 currentPosition = lseek(gtFile->sys_file, 0, SEEK_CUR);

	gt_int64 fileSize = lseek(gtFile->sys_file, 0, SEEK_END);

	gt_int64 newPosition = lseek(gtFile->sys_file, currentPosition, SEEK_SET);

	if (gtFile == NULL) return GT_ERROR_PARAMETER_0;
	if (gtFile->magic != GT_MAGIC_FILE) return GT_ERROR_PARAMETER_0;

	if (gtFile->sys_file < 0) {
		return GT_ERROR_OPEN_FILE;
	}

	

	if (currentPosition != newPosition) {
		return GT_ERROR;
	}

	return fileSize;
}

int GtFile_Chmod(gt_utf8 *path, int mode) {
	int ret ;
	
	if (path == NULL) return GT_ERROR_PARAMETER_0;

	ret = chmod((const char *)path, mode);
	if (ret != 0) {
		return GT_ERROR;
	}

	return GT_OK;
}

#endif

#ifdef __cplusplus
}
#endif
