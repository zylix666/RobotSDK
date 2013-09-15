/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_hash.h"

#include "sha1.h"
#include "sha256.h"
#include "md5.h"

#ifdef __cplusplus
extern "C" 
{
#endif

int GtMd5_GetString(char *str, int strSize, void *data, int dataLength) {


	unsigned char res[20];
	MD5_CTX md5;
	
	int i;
	if (str == NULL) return GT_ERROR_PARAMETER_0;
	if (strSize < 33) return GT_ERROR_PARAMETER_1;
	if (data == NULL) return GT_ERROR_PARAMETER_2;
	if (dataLength <= 0) return GT_ERROR_PARAMETER_3;




	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)data, dataLength);
	MD5Final(res,&md5);

	for (i = 0; i < 16; i++) {
		sprintf(str + (i * 2), "%02x", res[i]);
	}
	
	return 33;
}

int GtSha1_GetString(char *str, int strSize, void *data, int dataLength) {
	SHA1Context sha;
	int i;
	if (str == NULL) return GT_ERROR_PARAMETER_0;
	if (strSize < 41) return GT_ERROR_PARAMETER_1;
	if (data == NULL) return GT_ERROR_PARAMETER_2;
	if (dataLength <= 0) return GT_ERROR_PARAMETER_3;


	SHA1Reset(&sha);
	SHA1Input(&sha, (const unsigned char *)data, dataLength);
	SHA1Result(&sha);


	for (i = 0; i < 5; i++) {
		sprintf(str + (i * 8), "%08x", sha.Message_Digest[i]);
	}
	
	return 41;
}

int GtSha256_GetString(char *str, int strSize, void *data, int dataLength) {
	unsigned char res[40];
	SHA256_CTX ctx;
	int i;
	if (str == NULL) return GT_ERROR_PARAMETER_0;
	if (strSize < 65) return GT_ERROR_PARAMETER_1;
	if (data == NULL) return GT_ERROR_PARAMETER_2;
	if (dataLength <= 0) return GT_ERROR_PARAMETER_3;
	

	

	sha256_init(&ctx);
	sha256_update(&ctx, (unsigned char *)data, dataLength);
	sha256_final(&ctx,res);


	for (i = 0; i < 32; i++) {
		sprintf(str + (i * 2), "%02x", res[i]);
	}

	return 65;
}

#ifdef __cplusplus
}
#endif
