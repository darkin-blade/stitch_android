#include <android/log.h>
#include <android/bitmap.h>

#include <jni.h>

#include <string.h>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include "opencv2/stitching/detail/timelapsers.hpp"

#define LOG(format, ...) __android_log_print(ANDROID_LOG_INFO, "fuck", "[%s, %d] " format, __func__, __LINE__, ## __VA_ARGS__)

using namespace std;
using namespace cv;
using namespace cv::detail;

// 特征点匹配
// 绘制接缝线

// Default command line args
vector<String> img_names;
bool preview = false;
bool try_cuda = false;
double work_megapix = 0.6;
double seam_megapix = 0.1;
double compose_megapix = -1;
float conf_thresh = 1.f;
#ifdef HAVE_OPENCV_XFEATURES2D
string features_type = "surf";
#else
string features_type = "orb";
#endif
string matcher_type = "homography";
string estimator_type = "homography";
string ba_cost_func = "ray";
string ba_refine_mask = "xxxxx";
bool do_wave_correct = true;
WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;
bool save_graph = false;
std::string save_graph_to;
string warp_type = "spherical";
int expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
int expos_comp_nr_feeds = 1;
int expos_comp_nr_filtering = 2;
int expos_comp_block_size = 32;
float match_conf = 0.3f;
string seam_find_type = "gc_color";
int blend_type = Blender::MULTI_BAND;
int timelapse_type = Timelapser::AS_IS;
float blend_strength = 5;
string result_name = "result.jpg";
bool timelapse = false;
int range_width = -1;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_stitch_MainActivity_stitch_1e(
        JNIEnv *env,
        jobject thiz,
        jstring featuresType,
        jstring warpType,
        jlong result) {
    // 接收所有参数
    features_type = env->GetStringUTFChars(featuresType, 0);
    warp_type = env->GetStringUTFChars(warpType, 0);

    LOG("%s %s", features_type.data(), warp_type.data());

//    *(Mat *)result = nullptr;

//    jsize str_len = env->GetArrayLength(imgPaths);
//
//    for (int i = 0; i < str_len; i ++) {
//        jstring tmp = (jstring) env->GetObjectArrayElement(imgPaths, i);
//        const char *img_path = env->GetStringUTFChars(tmp, 0);
//
//        LOG("img[%d]: %s", i, img_path);
//    }
}