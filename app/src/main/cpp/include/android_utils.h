//
// Created by danbogh on 2017/9/12.
//

#ifndef OPENCV_FACE_DETECTION_ANDROID_UTILS_H
#define OPENCV_FACE_DETECTION_ANDROID_UTILS_H

#include <android/bitmap.h>
#include <opencv2/opencv.hpp>

using namespace cv;

void bitmap_to_mat(JNIEnv *env, jobject &srcBitmap_, Mat &srcMat);

void mat_to_bitmap(JNIEnv *env, Mat &srcMat, jobject &dstBitmap_);

#endif //OPENCV_FACE_DETECTION_ANDROID_UTILS_H
