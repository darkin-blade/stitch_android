#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <string.h>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s] " format, __func__, ## __VA_ARGS__)

using namespace std;
using namespace cv;
using namespace cv::detail;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_findPoint(
        JNIEnv *env,
        jobject thiz,
        jstring imgPath,
        jlong result) {
    const char *img_name = env->GetStringUTFChars(imgPath, 0);// TODO 不能直接sprintf
    LOG("img name: %s", img_name);

    // 读取图片
    Mat img = imread(img_name);

    // 查找特征点
    Ptr<Feature2D> finder;
    vector<ImageFeatures> features(1);
    if (true) {
        finder = ORB::create();
    } else {
        finder = AKAZE::create();
    }
    finder->detectAndCompute(img, noArray(), features[0].keypoints, features[0].descriptors);

    // 绘制特征点
    if (true) {
        Mat *imgKeyPoint = (Mat *)result;
        drawKeypoints(img, features[0].keypoints, *imgKeyPoint, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    }

}