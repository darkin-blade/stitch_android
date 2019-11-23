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
JNIEXPORT Mat JNICALL
Java_com_example_stitch_MainActivity_findPoint(
        JNIEnv *env,
        jobject thiz,
        jstring imgPath) {
    char img_name[64];

    // 获取路径
    sprintf(img_name, "%s", env->GetStringUTFChars(imgPath, 0));
    LOG("img name: %s", img_name);

    // 读取图片
    Mat img = imread(img_name);

    // 查找特征点
    Ptr<Feature2D> finder;
    vector<KeyPoint> keyPoints;
    if (true) {
        finder = ORB::create();
    } else {
        finder = AKAZE::create();
    }
    finder->detect(img, keyPoints);

    // 绘制特征点
    Mat imgKeyPoint;
    drawKeypoints(img, keyPoints, imgKeyPoint, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    return imgKeyPoint;

}