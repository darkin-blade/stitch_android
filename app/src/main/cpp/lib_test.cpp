#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s] " format, __func__, ## __VA_ARGS__)

using namespace cv;

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

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_example_stitch_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello;

    if (true) {
        hello = "Hello from OpenCV " + (std::string) CV_VERSION;
    } else {
        hello = "Hello from C++";
    }

    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_path2Bmp(
        JNIEnv *env,
        jobject thiz,
        jstring imgPath,
        jobject img_send) {
    // 获取路径
    const char* img_path = env->GetStringUTFChars(imgPath, 0);
    LOG("%s\n", img_path);

    Mat src = imread(img_path);
    AndroidBitmapInfo info;
    void *pixels = 0;

    CV_Assert(AndroidBitmap_getInfo(env, img_send, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(src.dims == 2 && info.height == (uint32_t) src.rows &&
              info.width == (uint32_t) src.cols);
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
    CV_Assert(AndroidBitmap_lockPixels(env, img_send, &pixels) >= 0);
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

    AndroidBitmap_unlockPixels(env, img_send);
}

JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_path2Size(
        JNIEnv *env,
        jobject thiz,
        jstring imgPath,
        jobject img_size) {
    // 获得图片尺寸
    // 获取路径
    const char* img_path = env->GetStringUTFChars(imgPath, 0);
    LOG("%s\n", img_path);

    Mat img = imread(img_path);
    int width = img.cols;// 宽
    int height = img.rows;// 高

    // 获取对象
    jclass tmp_class = env->GetObjectClass(img_size);
    jfieldID id;

    // 设置宽高
    id = env->GetFieldID(tmp_class, "width", "I");
    env->SetIntField(img_size, id, width);
    id = env->GetFieldID(tmp_class, "height", "I");
    env->SetIntField(img_size, id, height);
}

}
