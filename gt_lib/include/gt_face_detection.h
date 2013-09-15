///*
// Copyright (c) LEADERG Co., Ltd. All rights reserved.
// Free of charge for noncommercial purposes.
// Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
// */
//
//#ifndef __GT_FACE_DETECTION_H__
//#define __GT_FACE_DETECTION_H__
//
//#include "gt_lib.h"
//
//#ifdef __cplusplus
//extern "C" 
//{
//#endif
//
//typedef struct {
//	int x;
//	int y;
//	int width;
//	int height;
//} GtFaceDetectionPosition_t;
//
//typedef struct {
//    GtArray *faceDetectionPositions;
//
//    void        *lock;
//    GT_MAGIC_t   magic;
//} GtFaceDetection_t;
//
//
//int GtFaceDetection_InitLib();
//
//GtFaceDetection_t *GtFaceDetection_New();
//
//int GtFaceDetection_Free(GtFaceDetection_t *faceDetection);
//
//int GtFaceDetection_Lock(GtFaceDetection_t *faceDetection);
//
//int GtFaceDetection_Unlock(GtFaceDetection_t *faceDetection);
//
//int GtFaceDetection_Test(GtFaceDetection_t *faceDetection);
//
//int GtFaceDetection_Detect(GtFaceDetection_t *faceDetection, GtImage *image);
//
//int GtFaceDetection_GetFaceCount(GtFaceDetection_t *faceDetection);
//
//int GtFaceDetection_GetFace(GtFaceDetection_t *faceDetection, int sn, GtFaceDetectionPosition_t *faceDetectionPosition);
//
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif /* __GT_FACE_DETECTION_H__ */
