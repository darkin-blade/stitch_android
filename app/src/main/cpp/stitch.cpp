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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_matchPoint(
        JNIEnv *env,
        jobject thiz,
        jobjectArray imgPaths,
        jlong result) {
    int img_num = (int)env->GetArrayLength(imgPaths);
    LOG("%d", img_num);

    Ptr<Feature2D> finder;
    if (true) {
        finder = ORB::create();
    } else {
        finder = AKAZE::create();
    }

    Mat img;// 临时存储(缩放后的)图片
    vector<Mat> imgs;// 存储所有图片
    vector<ImageFeatures> features(img_num);

    // 寻找特征点
    for (int i = 0; i < img_num; i ++) {
        // TODO 读取图片
        jstring tmp = (jstring) env->GetObjectArrayElement(imgPaths, i);
        const char *img_path = env->GetStringUTFChars(tmp, 0);
        Mat full_img = imread(img_path);// 临时存放完整图片

        // 放缩图片, cols 是宽
        LOG("%s: [%d, %d]", img_path, full_img.cols, full_img.rows);
        int width = 480;
        int height = full_img.rows * 480 / full_img.cols;
        img = Mat(height, width, full_img.type());
        resize(full_img, img, img.size(), 0, 0, INTER_LINEAR);
        imgs.push_back(img);// 保存图片到向量

        // 查找特征点
        finder->detectAndCompute(img, noArray(), features[i].keypoints, features[i].descriptors);
        // computeImageFeatures(finder, img, features[i]);

        // 绘制特征点
        if (true) {
            Mat imgKeyPoints;
            drawKeypoints(img, features[i].keypoints, imgKeyPoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        }

        full_img.release();
    }

    // 匹配特征点
    vector<MatchesInfo> pairwise_matches;

    // 第三种匹配方法, 参考: https://www.jianshu.com/p/420f8211d1cb
    vector<Mat> imgMatches(img_num);// 记录特征点信息
    Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create("BruteForce-Hamming");// 使用汉明距离进行匹配

    finder->compute(imgs[0], features[0].keypoints, imgMatches[0]);
    finder->compute(imgs[1], features[1].keypoints, imgMatches[1]);

    // 特征点匹配
    vector<DMatch> bad_matches, good_matches;
    descriptor_matcher->match(imgMatches[0], imgMatches[1], bad_matches);

    // 绘制匹配点
    Mat bad_match, good_match;
    drawMatches(imgs[0], features[0].keypoints, imgs[1], features[1].keypoints, bad_matches, bad_match);

    *(Mat *)result = bad_match;// TODO

    // TODO 全局匹配
    Ptr<FeaturesMatcher> matcher;
    if (false) {// 定义特征匹配器
        matcher = makePtr<BestOf2NearestMatcher>();// TODO 参数
    } else if (true) {
        matcher = makePtr<AffineBestOf2NearestMatcher>();// TODO 参数
    } else {
        assert(0);
    }

    (*matcher)(features, pairwise_matches);// 进行特征匹配
}