#include <jni.h>
#include <android/log.h>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s] " format, __func__, __VA_ARGS__)

using namespace cv;

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

JNIEXPORT jint JNICALL
Java_com_example_stitch_MainActivity_changeImg(
        JNIEnv *env,
        jobject thiz,
        jobject img_send) {
    // TODO: implement changeImg()

}

JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_getImgSize(
        JNIEnv *env,
        jobject thiz,
        jstring img_path,
        jobject img_size) {
    jclass tmp_class = env->GetObjectClass(img_size);
    jfieldID id = env->GetFieldID(tmp_class, "width", "I");
    jint width = env->GetIntField(img_size, id);
    LOG("%ld\n", width);
    // TODO: implement getImgSize()
}

}
