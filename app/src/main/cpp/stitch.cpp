#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <vector>

#include <opencv2/opencv.hpp>

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s] " format, __func__, ## __VA_ARGS__)

using namespace std;
using namespace cv;
using namespace cv::detail;



extern "C"
JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_mainTest(JNIEnv *env, jobject thiz, jstring img) {
    char tmp_name[32];// 临时文件名
    const int img_num = 2;
    const char *img_name[img_num] = {"mac1.jpg", "mac2.jpg"};

    Ptr<Feature2D> finder;
    if (true) {
        finder = ORB::create();
    } else {
        finder = AKAZE::create();
    }

    Mat img;
    vector<Mat> imgs;// 存储所有图片
    vector<ImageFeatures> features(img_num);

    // 寻找特征点
    for (int i = 0; i < img_num; i ++) {
        // 读取图片
        Mat full_img = imread(img_name[i]);

        // 放缩图片, cols 是宽
        int width = 480;
        int height = full_img.rows * 480 / full_img.cols;
        img = Mat(height, width, full_img.type());
        resize(full_img, img, img.size(), 0, 0, INTER_LINEAR);
        imgs.push_back(img);// 保存图片到向量

        // 查找特征点
        computeImageFeatures(finder, img, features[i]);

        // 绘制特征点
        if (false) {
            Mat imgKeyPoints;
            drawKeypoints(img, features[i].keypoints, imgKeyPoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
            imshow(img_name[i], imgKeyPoints);

            sprintf(tmp_name, "tmp%d.jpg", i);
            imwrite(tmp_name, imgKeyPoints);
        }

        full_img.release();
    }

    img.release();
}