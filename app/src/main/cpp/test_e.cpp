#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <string.h>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s, %d] " format, __func__, __LINE__, ## __VA_ARGS__)

using namespace std;
using namespace cv;
using namespace cv::detail;

// 特征点匹配
// 绘制接缝线

extern "C"
JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_stitch_1e(
        JNIEnv *env,
        jobject thiz,
        jstring featuresType) {
    // 接收所有参数
    const char *features_type = env->GetStringUTFChars(featuresType, 0);
    LOG("features type: %s", features_type);

//    jsize str_len = env->GetArrayLength(imgPaths);
//
//    for (int i = 0; i < str_len; i ++) {
//        jstring tmp = (jstring) env->GetObjectArrayElement(imgPaths, i);
//        const char *img_path = env->GetStringUTFChars(tmp, 0);
//
//        LOG("img[%d]: %s", i, img_path);
//    }
}