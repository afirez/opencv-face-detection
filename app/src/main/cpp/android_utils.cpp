//
// Created by danbogh on 2017/9/12.
//
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "android_utils.h"

void bitmap_to_mat(JNIEnv *env, jobject &srcBitmap_, Mat &srcMat) {
    void *srcPixels = 0;
    AndroidBitmapInfo srcBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, srcBitmap_, &srcBitmapInfo);
        AndroidBitmap_lockPixels(env, srcBitmap_, &srcPixels);
        uint32_t srcHeight = srcBitmapInfo.height;
        uint32_t srcWidth = srcBitmapInfo.width;
        srcMat.create(srcHeight, srcWidth, CV_8UC4);
        if (srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tempMat(srcHeight, srcWidth, CV_8UC4, srcPixels);
            tempMat.copyTo(srcMat);
        } else {
            Mat tempMat = Mat(srcHeight, srcWidth, CV_8UC2, srcPixels);
            cv::cvtColor(tempMat, srcMat, COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, srcBitmap_);
        return;
    } catch (Exception &e) {
        AndroidBitmap_unlockPixels(env, srcBitmap_);
        jclass class_Exception = env->FindClass("java/lang/Exception");
        env -> ThrowNew(class_Exception, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, srcBitmap_);
        jclass class_Exception = env->FindClass("java/lang/Exception");
        env -> ThrowNew(class_Exception, "unknown");
        return;
    }
}

void mat_to_bitmap(JNIEnv *env, Mat &srcMat, jobject &dstBitmap_) {
    void *dstPixels = 0;
    AndroidBitmapInfo dstBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, dstBitmap_, &dstBitmapInfo);
        AndroidBitmap_lockPixels(env, dstBitmap_, &dstPixels);
        uint32_t dstHeight = dstBitmapInfo.height;
        uint32_t dstWidth = dstBitmapInfo.width;
        if (dstBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tmp(dstHeight, dstWidth, CV_8UC4, dstPixels);
            if(srcMat.type() == CV_8UC1) {
                cv::cvtColor(srcMat, tmp, COLOR_GRAY2RGBA);
            } else if (srcMat.type() == CV_8UC3) {
                cv::cvtColor(srcMat, tmp, COLOR_RGB2RGBA);
            } else if (srcMat.type() == CV_8UC4) {
                srcMat.copyTo(tmp);
            }
        } else {
            Mat tmp = Mat(dstHeight, dstWidth, CV_8UC2, dstPixels);
            if(srcMat.type() == CV_8UC1) {
                cv::cvtColor(srcMat, tmp, COLOR_GRAY2BGR565);
            } else if (srcMat.type() == CV_8UC3) {
                cv::cvtColor(srcMat, tmp, COLOR_RGB2BGR565);
            } else if (srcMat.type() == CV_8UC4) {
                cv::cvtColor(srcMat, tmp, COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, dstBitmap_);
    }catch (Exception &e) {
        AndroidBitmap_unlockPixels(env, dstBitmap_);
        jclass class_Exception = env->FindClass("java/lang/Exception");
        env -> ThrowNew(class_Exception, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, dstBitmap_);
        jclass class_Exception = env->FindClass("java/lang/Exception");
        env -> ThrowNew(class_Exception, "unknown");
        return;
    }
}