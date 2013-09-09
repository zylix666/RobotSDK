/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#include "gt_lib.h"
#include "gt_image.h"
#include "gt_image_barcode.h"
#include "gt_bitmap.h"

#include <glue.h>

// android specific headers
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

// standard library
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>

#include "zbar.h"

JNIEXPORT jint JNICALL Java_com_leaderg_gt_lib_GtLib_initJni(JNIEnv *env, jobject obj) {
	GT_LOGI("initJni");

	int ret;

	ret = GtLib_InitLib("GT LICENSE TRIAL UNTIL 2014", NULL);
	if (ret <= 0) {
		GT_LOGE("GtLib_InitLib = %d", ret);
		return ret;
	}

	ret = GtImage_InitLib("GT LICENSE TRIAL UNTIL 2014", NULL);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitLib = %d", ret);
		return ret;
	}

	ret = GtBitmap_InitLib("GT LICENSE TRIAL UNTIL 2014", NULL);
	if (ret <= 0) {
		GT_LOGE("GtBitmap_InitLib = %d", ret);
		return ret;
	}

	ret = GtImageBarcode_InitLib();
	if (ret <= 0) {
		GT_LOGE("GtImageBarcode_InitLib = %d", ret);
		return ret;
	}

	ret = GtSvm_InitLib("GT LICENSE TRIAL UNTIL 2014", NULL);
	if (ret <= 0)
	{
		GT_LOGE("GtSvm_InitLib = %d", ret);
		return 1;
	}

	return ret;
}

JNIEXPORT jint JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_test(JNIEnv *env, jobject obj) {
	GT_LOGI("test ++");

	Glue_Test();

	GT_LOGI("test --");
	return 1;
}

JNIEXPORT jint JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_setThreshold(JNIEnv *env, jobject obj, jbyteArray baInputBuffer, jint colorMode, jint width, jint height, jint threshold, jbyteArray baOutputBuffer) {
	GT_LOGI("setThreshold ++");

	GT_LOGI("1");

	gt_byte *inputBuffer = (*env)->GetByteArrayElements(env, baInputBuffer, 0);
	int inputBufferLength = (*env)->GetArrayLength(env, baInputBuffer);
	gt_byte *outputBuffer = (*env)->GetByteArrayElements(env, baOutputBuffer, 0);
	int outputBufferLength = (*env)->GetArrayLength(env, baOutputBuffer);

	GT_LOGI("2");

	int ret;

	GtImage_t *image = GtImage_New();

	GT_LOGI("3");

	ret = GtImage_InitImage(image, width, height, GT_COLOR_MODE_RGB888);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}

	GT_LOGI("4");

	ret = GtImage_Load(image, inputBuffer, inputBufferLength, colorMode, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Load = %d", ret);
		return ret;
	}

	GT_LOGI("5");

	ret = GtImage_SetColorMode(image, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetColorMode = %d", ret);
		return ret;
	}

	GT_LOGI("6");

	ret = GtImage_SetThreshold(image, 128);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetThreshold = %d", ret);
		return ret;
	}

	GT_LOGI("7");

	GT_LOGI("colorMode = %d", colorMode);
	GT_LOGI("outputBufferLength = %d", outputBufferLength);
	GT_LOGI("width = %d", width);
	GT_LOGI("height = %d", height);

	ret = GtImage_Save(image, outputBuffer, outputBufferLength, GT_COLOR_MODE_ARGB8888, width, height);
	//ret = GtImage_Save(image, outputBuffer, outputBufferLength, GT_COLOR_MODE_RGB565, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Save = %d", ret);
		return ret;
	}

	GT_LOGI("8");

	GtImage_Free(image);

	GT_LOGI("9");

	(*env)->ReleaseByteArrayElements(env, baInputBuffer, inputBuffer, 0);
	(*env)->ReleaseByteArrayElements(env, baOutputBuffer, outputBuffer, 0);

	GT_LOGI("setThreshold --");
	return ret;

	//GT_LOGI("setThreshold --");
	//return 0;
}

JNIEXPORT jstring JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_getBarCode(JNIEnv *env, jobject obj, jbyteArray baPixels, jint colorMode, jint width, jint height) {
	GT_LOGI("getBarCode 1");

	gt_byte *buffer = (*env)->GetByteArrayElements(env, baPixels, 0);
	int bufferLength = (*env)->GetArrayLength(env, baPixels);

	GT_LOGI("bufferLength = %d", bufferLength);
	GT_LOGI("colorMode = %d", colorMode);
	GT_LOGI("width = %d", width);
	GT_LOGI("height = %d", height);

	GtImage_t *image = GtImage_New();
	if (image == NULL) {
		GT_LOGE("image == NULL");
		return NULL;
	}

	int ret;
	ret = GtImage_InitImage(image, width, height, GT_COLOR_MODE_RGB888);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return NULL;
	}

	ret = GtImage_Load(image, buffer, bufferLength, colorMode, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Load = %d", ret);
		return NULL;
	}

	ret = GtImage_SetColorMode(image, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetColorMode = %d", ret);
		return NULL;
	}

	char code[1024];
	code[0] = '\0';
	ret = GtImageBarcode_Decode(image, code, 1024);
	if (ret <= 0) {
		GT_LOGE("GtImageBarcode_Decode = %d", ret);
		return NULL;
	}

	GtImage_Free(image);

	(*env)->ReleaseByteArrayElements(env, baPixels, buffer, 0);

	if(strlen(code) <= 0) {
		return NULL;
	}

	jclass strClass = (*env)->FindClass(env, "java/lang/String");
	jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(code));
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(code), (jbyte*)code);
	jstring encoding = (*env)->NewStringUTF(env, "utf-8");
	return (jstring)(*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

JNIEXPORT jstring JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_getFaceName(JNIEnv *env, jobject obj, jbyteArray baPixels, jint colorMode, jint width, jint height) {
	GT_LOGI("getFaceName");

	// TODO


	char *barCode = "Name";
	jclass strClass = (*env)->FindClass(env, "Ljava/lang/String;");
	jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(barCode));
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(barCode), (jbyte*)barCode);
	jstring encoding = (*env)->NewStringUTF(env, "utf-8");
	return (jstring)(*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

JNIEXPORT jint JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_getDepthImage(JNIEnv *env, jobject obj, jbyteArray baBufferL, jbyteArray baBufferR, jint colorMode, jint width, jint height, jbyteArray baBufferDepth) {
	
	
	GT_LOGI("getDepthImage ++");
	
	GT_LOGI("1");
	
	GtImage_t *imageL = GtImage_New();
	if (imageL == NULL) {
		GT_LOGE("imageL == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	GtImage_t *imageR = GtImage_New();
	if (imageR == NULL) {
		GT_LOGE("imageR == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	GtImage_t *imageDepth = GtImage_New();
	if (imageDepth == NULL) {
		GT_LOGE("imageDepth == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	GtImage_t *imageBlob = GtImage_New();
	if (imageBlob == NULL) {
		GT_LOGE("imageBlob == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	GT_LOGI("2");
	
	
	gt_byte *bufferL = (*env)->GetByteArrayElements(env, baBufferL, 0);
	int bufferLengthL = (*env)->GetArrayLength(env, baBufferL);

	gt_byte *bufferR = (*env)->GetByteArrayElements(env, baBufferR, 0);
	int bufferLengthR = (*env)->GetArrayLength(env, baBufferR);

	gt_byte *bufferDepth = (*env)->GetByteArrayElements(env, baBufferDepth, 0);
	int bufferDepthLength = (*env)->GetArrayLength(env, baBufferDepth);

	GT_LOGI("3");
	
	int ret;

	ret = GtImage_InitImage(imageL, width, height, GT_COLOR_MODE_RGB888);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}
	
	GT_LOGI("4");

	ret = GtImage_InitImage(imageR, width, height, GT_COLOR_MODE_RGB888);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}
	
	GT_LOGI("5");

	ret = GtImage_InitImage(imageDepth, width, height, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}
	
	ret = GtImage_InitImage(imageBlob, width, height, GT_COLOR_MODE_INT32);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}

	
	GT_LOGI("6");

	ret = GtImage_Load(imageL, bufferL, bufferLengthL, colorMode, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Load = %d", ret);
		return ret;
	}
	
	GT_LOGI("7");

	ret = GtImage_Load(imageR, bufferR, bufferLengthR, colorMode, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Load = %d", ret);
		return ret;
	}
	
	
	GT_LOGI("8");

	ret = GtImage_SetColorMode(imageL, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetColorMode = %d", ret);
		return ret;
	}
	
	
	GT_LOGI("9");

	ret = GtImage_SetColorMode(imageR, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetColorMode = %d", ret);
		return ret;
	}

	GT_LOGI("GtImage_GetDepthImage");

	ret = GtImage_GetDepthImage(imageDepth, imageL, imageR);
	if (ret <= 0) {
		GT_LOGE("GtImage_GetDepthImage = %d", ret);
		return ret;
	}

	/*
	ret = GtImage_Blob(imageBlob, imageDepth, 20);
	if (ret <= 0) {
		GT_LOGE("GtImage_Blob = %d", ret);
		return ret;
	}

	ret = GtImage_ConvertBlobToGray8(imageDepth, imageBlob);
	if (ret <= 0) {
		GT_LOGE("GtImage_ConvertBlobToGray8 = %d", ret);
		return ret;
	}
	*/

	ret = GtImage_Save(imageDepth, bufferDepth, bufferDepthLength, GT_COLOR_MODE_ARGB8888, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Save = %d", ret);
		return ret;
	}

	GtImage_Free(imageL);
	GtImage_Free(imageR);
	GtImage_Free(imageDepth);
	(*env)->ReleaseByteArrayElements(env, baBufferL, bufferL, 0);
	(*env)->ReleaseByteArrayElements(env, baBufferR, bufferR, 0);
	(*env)->ReleaseByteArrayElements(env, baBufferDepth, bufferDepth, 0);

	GT_LOGI("getDepthImage --");
	return ret;
}

JNIEXPORT jint JNICALL Java_com_leaderg_gt_lib_GtTemplateJni_edge(JNIEnv *env, jobject obj, jbyteArray baInputBuffer, jint colorMode, jint width, jint height, jint threshold, jbyteArray baOutputBuffer) {
	gt_byte *inputBuffer = (*env)->GetByteArrayElements(env, baInputBuffer, 0);
	int inputBufferLength = (*env)->GetArrayLength(env, baInputBuffer);
	gt_byte *outputBuffer = (*env)->GetByteArrayElements(env, baOutputBuffer, 0);
	int outputBufferLength = (*env)->GetArrayLength(env, baOutputBuffer);

	int ret;

	GtImage_t *imageL = GtImage_New();
	if (imageL == NULL) {
		GT_LOGE("imageL == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	GtImage_t *imageBlob = GtImage_New();
	if (imageBlob == NULL) {
		GT_LOGE("imageBlob == NULL");
		return GT_ERROR_NO_MEMORY;
	}

	ret = GtImage_InitImage(imageL, width, height, GT_COLOR_MODE_RGB888);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}

	ret = GtImage_Load(imageL, inputBuffer, inputBufferLength, colorMode, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Load = %d", ret);
		return ret;
	}

	ret = GtImage_SetColorMode(imageL, GT_COLOR_MODE_GRAY8);
	if (ret <= 0) {
		GT_LOGE("GtImage_SetColorMode = %d", ret);
		return ret;
	}

	ret = GtImage_InitImage(imageBlob, width, height, GT_COLOR_MODE_INT32);
	if (ret <= 0) {
		GT_LOGE("GtImage_InitImage = %d", ret);
		return ret;
	}

	ret = GtImage_Blob(imageBlob, imageL, 20);
	if (ret <= 0) {
		GT_LOGE("GtImage_Blob = %d", ret);
		return ret;
	}

	ret = GtImage_ConvertBlobToGray8(imageL, imageBlob);
	if (ret <= 0) {
		GT_LOGE("GtImage_ConvertBlobToGray8 = %d", ret);
		return ret;
	}

	ret = GtImage_Save(imageL, outputBuffer, outputBufferLength, GT_COLOR_MODE_ARGB8888, width, height);
	if (ret <= 0) {
		GT_LOGE("GtImage_Save = %d", ret);
		return ret;
	}

	GtImage_Free(imageL);

	(*env)->ReleaseByteArrayElements(env, baInputBuffer, inputBuffer, 0);
	(*env)->ReleaseByteArrayElements(env, baOutputBuffer, outputBuffer, 0);

	return ret;
}

static int registerMethods(JNIEnv* env) {
	GT_LOGI("registerMethods");

	jclass classFind;

	classFind = (*env)->FindClass(env, "com/leaderg/gt_lib/GtLibJni");
	if (classFind == NULL) {
		return -1;
	}

	static JNINativeMethod methodGtLibJni[] = { { "initJni", "()I", (void *) Java_com_leaderg_gt_lib_GtLib_initJni }, };
	if ((*env)->RegisterNatives(env, classFind, methodGtLibJni, sizeof(methodGtLibJni) / sizeof(methodGtLibJni[0])) != JNI_OK) {
		return -2;
	}

	classFind = (*env)->FindClass(env, "com/leaderg/gt_lib/GtTemplateJni");
	if (classFind == NULL) {
		return -1;
	}

	static JNINativeMethod methodGtTemplateJni[] = {
			{ "setThreshold", "([BIIII[B)I", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_setThreshold},
			{ "getBarCode", "([BIII)Ljava/lang/String;", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_getBarCode},
			{ "getFaceName", "([BIII)Ljava/lang/String;", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_getFaceName},
			{ "getDepthImage", "([B[BIII[B)I", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_getDepthImage},
			{ "edge", "([BIIII[B)I", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_edge},
			{ "test", "()I", (void *) Java_com_leaderg_gt_lib_GtTemplateJni_test}
	};
	if ((*env)->RegisterNatives(env, classFind, methodGtTemplateJni, sizeof(methodGtTemplateJni) / sizeof(methodGtTemplateJni[0])) != JNI_OK) {
		return -2;
	}

	return 1;
}

// This is called by the VM when the shared library is first loaded.
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	GT_LOGI("JNI_OnLoad built on %s %s", __DATE__, __TIME__);

	JNIEnv* env = NULL;

	jint result = JNI_VERSION_1_4;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		GT_LOGE("!= JNI_VERSION_1_4");
		result = -1;
	}
	if (registerMethods(env) != 1) {
		GT_LOGE("registerMethods(env) != 1");
		result = -1;
	}

	GT_LOGI("JNI_OnLoad result=%d", result);
	return result;
}
