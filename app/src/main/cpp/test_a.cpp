#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s] " format, __func__, ## __VA_ARGS__)

using namespace cv;
using namespace detail;

// mat转bmp

void mat2bmp(Mat src, jobject dest, JNIEnv *env) {
    AndroidBitmapInfo info;
    void *pixels = 0;

    CV_Assert(AndroidBitmap_getInfo(env, dest, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(src.dims == 2 && info.height == (uint32_t) src.rows &&
              info.width == (uint32_t) src.cols);
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
    CV_Assert(AndroidBitmap_lockPixels(env, dest, &pixels) >= 0);
    CV_Assert(pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOG("8888 format");
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (src.type() == CV_8UC1) {
            LOG("nMatToBitmap: CV_8UC1 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_GRAY2RGBA);
        } else if (src.type() == CV_8UC3) {
            LOG("nMatToBitmap: CV_8UC3 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_BGR2RGBA);
        } else if (src.type() == CV_8UC4) {// TODO BGR
            LOG("nMatToBitmap: CV_8UC4 -> RGBA_8888");
            if (true)  // needPremultiplyAlpha: TODO 是否前乘透明度
                cvtColor(src, tmp, COLOR_RGBA2mRGBA);
            else
                src.copyTo(tmp);
        }
    } else {
        LOG("%d", info.format == ANDROID_BITMAP_FORMAT_RGB_565);// TODO
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (src.type() == CV_8UC1) {
            LOG("nMatToBitmap: CV_8UC1 -> RGB_565");
            cvtColor(src, tmp, COLOR_GRAY2BGR565);
        } else if (src.type() == CV_8UC3) {// TODO BGR
            LOG("nMatToBitmap: CV_8UC3 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGB2BGR565);
        } else if (src.type() == CV_8UC4) {// TODO BGR
            LOG("nMatToBitmap: CV_8UC4 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGBA2BGR565);
        }
    }

    AndroidBitmap_unlockPixels(env, dest);
}