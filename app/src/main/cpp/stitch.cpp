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
         computeImageFeatures(finder, img, features[i]);

        // 绘制特征点
        if (true) {
            Mat imgKeyPoints;
            drawKeypoints(img, features[i].keypoints, imgKeyPoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        }

        full_img.release();
    }

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

    // 匹配特征点
    vector<MatchesInfo> pairwise_matches;
    Ptr<FeaturesMatcher> matcher;
    if (false) {// 定义特征匹配器
        matcher = makePtr<BestOf2NearestMatcher>();// TODO 参数
    } else if (true) {
        matcher = makePtr<AffineBestOf2NearestMatcher>();// TODO 参数
    } else {
        assert(0);
    }

    (*matcher)(features, pairwise_matches);// 进行特征匹配

    // 相机参数评估
    Ptr<Estimator> estimator;
    if (true) {
        estimator = makePtr<HomographyBasedEstimator>();
    } else if (false) {
        estimator = makePtr<AffineBasedEstimator>();
    } else {
        assert(0);
    }

    vector<CameraParams> cameras;
    (*estimator)(features, pairwise_matches, cameras);
    for (size_t i = 0; i < cameras.size(); ++i)
    {
        Mat R;
        cameras[i].R.convertTo(R, CV_32F);
        cameras[i].R = R;
    }

    Ptr<detail::BundleAdjusterBase> adjuster;// 光束平差法,精确化相机参数
    if (false) adjuster = makePtr<detail::BundleAdjusterReproj>();
    else if (true) adjuster = makePtr<detail::BundleAdjusterRay>();
    else if (false) adjuster = makePtr<detail::BundleAdjusterAffinePartial>();
    adjuster->setConfThresh(1);// TODO conf_thresh
    // TODO setRefinementMask
    (*adjuster)(features, pairwise_matches, cameras);

    // 波形矫正
    vector<Mat> rmats;
    for (size_t i = 0; i < cameras.size(); i ++) {
        rmats.push_back(cameras[i].R.clone());
    }
    if (false) {// 水平校正
        waveCorrect(rmats, WAVE_CORRECT_HORIZ);
    } else if (true) {// 垂直校正
        waveCorrect(rmats, WAVE_CORRECT_VERT);
    } else {
        assert(0);
    }
    for (size_t i = 0; i < cameras.size(); i ++) {
        cameras[i].R = rmats[i];
    }

    // 图像映射变换
    vector<Point> corners(img_num);
    vector<UMat> masks_warped(img_num);
    vector<UMat> images_warped(img_num);
    vector<Mat> masks(img_num);

    for (int i = 0; i < img_num; i ++)
    {
        masks[i].create(imgs[i].size(), CV_8U);
        masks[i].setTo(Scalar::all(255));
    }

    Ptr<WarperCreator> warper_creator;
    // 选择投影类型
    if (true) {
        warper_creator = makePtr<cv::PlaneWarper>();
    } else if (false) {
        warper_creator = makePtr<cv::AffineWarper>();
    } else if (false) {
        warper_creator = makePtr<cv::CylindricalWarper>();
    } else if (false) {
        warper_creator = makePtr<cv::SphericalWarper>();
    } else {
        assert(0);
    }

    Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(cameras[0].focal));
    for (int i = 0; i < img_num; i ++)
    {
        Mat_<float> K;
        cameras[i].K().convertTo(K, CV_32F);
        corners[i] = warper->warp(imgs[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
        warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
    }

    vector<UMat> images_warped_f(img_num);// TODO
    for (int i = 0; i < img_num; i ++) {// 图像数据类型转换
        images_warped[i].convertTo(images_warped_f[i], CV_32F);
    }

    // 曝光补偿
    Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(ExposureCompensator::GAIN);// TODO 参数
    compensator->feed(corners, images_warped, masks_warped);

    for(int i = 0; i < img_num; i ++) {
        compensator->apply(i, corners[i], images_warped[i], masks_warped[i]);
    }

    Ptr<SeamFinder> seam_finder;// 接缝线寻找器
    if (true) {
        seam_finder = makePtr<detail::VoronoiSeamFinder>();// 逐点法
    } else if (false) {
        seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);// 默认: 图割法
    } else if (false) {
        seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
    } else if (false) {
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);// 动态规划法
    } else if (false) {
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
    } else {
        assert(0);
    }

    // TODO 释放内存

    // 得到接缝线的掩码图像 masks_warped
    seam_finder->find(images_warped_f, corners, masks_warped);

    // 参考: https://blog.csdn.net/zhaocj/article/details/78944867
    // 通过canny边缘检测，得到掩码边界，其中有一条边界就是接缝线
    for(int k = 0; k < img_num; k ++) {
        Canny(masks_warped[k], masks_warped[k], 3, 9, 3); // TODO 参数
    }

    // 为了使接缝线看得更清楚，这里使用了膨胀运算来加粗边界线
    vector<Mat> dilate_img(img_num);
    Mat element = getStructuringElement(MORPH_RECT, Size(4, 4));// 定义结构元素,粗细

    for(int i = 0; i < img_num; i ++)// 遍历所有图像
    {
        dilate(masks_warped[i], dilate_img[i],element);// 膨胀运算
        // 在映射变换图上画出接缝线,在这里只是为了呈现出的一种效果,所以并没有区分接缝线和其他掩码边界
        for(int y = 0; y < images_warped[i].rows; y ++) {
            for(int x = 0; x < images_warped[i].cols; x ++) {
                if (dilate_img[i].at<uchar>(y, x) == 255) {
                    // 掩码边界
                    // 将UMat转成Mat
                    // 颜色
                    images_warped[i].getMat(ACCESS_READ).at<Vec3b>(y, x)[0] = 0;// B
                    images_warped[i].getMat(ACCESS_READ).at<Vec3b>(y, x)[1] = 0;// G
                    images_warped[i].getMat(ACCESS_READ).at<Vec3b>(y, x)[2] = 255;// R
                }
            }
        }
    }

    Mat tmp = images_warped[0].getMat(ACCESS_READ);// images_warped[0].getMat(ACCESS_READ);
//    images_warped[0].copyTo(*(Mat *)result);
    *(Mat *)result = tmp.clone();
    LOG("Mat size:[%d, %d]", (*(Mat *)result).cols, (*(Mat *)result).rows);// TODO delete
    return;

}