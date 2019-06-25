//
// Created by danbogh on 2017/9/12.
//

#include <jni.h>
#include <string>
#include <android_utils.h>
#include <scanner.h>

static const char *const kClassDocScanner = "io/danbogh/facedetection/SmartCropper";

static struct {
    jclass class_Point;
    jmethodID methodID_init;
    jfieldID fieldID_x;
    jfieldID fieldID_y;
} gPointInfo;

static void initPointInfo(JNIEnv *env) {
    gPointInfo.class_Point = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("android/graphics/Point")));
    gPointInfo.methodID_init = env->GetMethodID(gPointInfo.class_Point, "<init>", "(II)V");
    gPointInfo.fieldID_x = env->GetFieldID(gPointInfo.class_Point, "x", "I");
    gPointInfo.fieldID_y = env->GetFieldID(gPointInfo.class_Point, "y", "I");
}

static jobject newPoint_(JNIEnv *env, Point point_) {
    return env->NewObject(gPointInfo.class_Point, gPointInfo.methodID_init, point_.x, point_.y);
}

static void native_scan(JNIEnv *env, jclass type, jobject srcBitmap_, jobjectArray outPoints_) {
    if (env->GetArrayLength(outPoints_) != 4) {
        return;
    }
    Mat srcBitmapMat;
    bitmap_to_mat(env, srcBitmap_, srcBitmapMat);
    Mat bgrMat(srcBitmapMat.rows, srcBitmapMat.cols, CV_8UC3);
    cv::cvtColor(srcBitmapMat, bgrMat, CV_RGBA2BGR);
    scanner::Scanner docScanner(bgrMat);
    vector<Point> scanPoints = docScanner.scanPoint();
    if (scanPoints.size() == 4) {
        for (int i = 0; i < 4; ++i) {
            env->SetObjectArrayElement(outPoints_, i, newPoint_(env, scanPoints[i]));
        }
    }
}

static vector<Point> pointsToNative(JNIEnv *env, jobjectArray points_) {
    int length = env->GetArrayLength(points_);
    vector<Point> points;
    for (int i = 0; i < length; i++) {
        jobject point_ = env->GetObjectArrayElement(points_, i);
        int x = env->GetIntField(point_, gPointInfo.fieldID_x);
        int y = env->GetIntField(point_, gPointInfo.fieldID_y);
        points.push_back(Point(x, y));
    }
    return points;
}

static void native_crop(
        JNIEnv *env, jclass type,
        jobject srcBitmap_,
        jobjectArray points_,
        jobject outBitmap_) {
    vector<Point> points = pointsToNative(env, points_);
    if (points.size() != 4) {
        return;
    }
    Point leftTop = points[0];
    Point rightTop = points[1];
    Point rightBottom = points[2];
    Point leftBottom = points[3];

    Mat srcBitmapMat;
    bitmap_to_mat(env, srcBitmap_, srcBitmapMat);

    AndroidBitmapInfo outBitmapInfo;
    AndroidBitmap_getInfo(env, outBitmap_, &outBitmapInfo);
    Mat dstBitmapMat;
    int newHeight = outBitmapInfo.height;
    int newWidth = outBitmapInfo.width;
    dstBitmapMat = Mat::zeros(newHeight, newWidth, srcBitmapMat.type());

    vector<Point2f> srcTriangle;
    vector<Point2f> dstTriangle;

    srcTriangle.push_back(Point2f(leftTop.x, leftTop.y));
    srcTriangle.push_back(Point2f(rightTop.x, rightTop.y));
    srcTriangle.push_back(Point2f(leftBottom.x, leftBottom.y));
    srcTriangle.push_back(Point2f(rightBottom.x, rightBottom.y));

    dstTriangle.push_back(Point2f(0, 0));
    dstTriangle.push_back(Point2f(newWidth, 0));
    dstTriangle.push_back(Point2f(0, newHeight));
    dstTriangle.push_back(Point2f(newWidth, newHeight));

    Mat transform = cv::getPerspectiveTransform(srcTriangle, dstTriangle);
    cv::warpPerspective(srcBitmapMat, dstBitmapMat, transform, dstBitmapMat.size());

    mat_to_bitmap(env, dstBitmapMat, outBitmap_);
}

static JNINativeMethod gMethods[] = {

        {
                "nativeScan",
                "(Landroid/graphics/Bitmap;[Landroid/graphics/Point;)V",
                (void *) native_scan
        },

        {
                "nativeCrop",
                "(Landroid/graphics/Bitmap;[Landroid/graphics/Point;Landroid/graphics/Bitmap;)V",
                (void *) native_crop
        }

};

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_FALSE;
    }
    jclass class_Scanner = env->FindClass(kClassDocScanner);
    if (env->RegisterNatives(
            class_Scanner,
            gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    initPointInfo(env);
    return JNI_VERSION_1_4;
}