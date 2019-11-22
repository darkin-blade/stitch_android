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
    return -1;
    // TODO: implement changeImg()
}

JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_getImgSize(
        JNIEnv *env,
        jobject thiz,
        jstring imgPath,
        jobject img_size) {
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
