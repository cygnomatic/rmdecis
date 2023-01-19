//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_TYPING_GENERAL_H
#define CYGNOIDES_TYPING_GENERAL_H

#include <iostream>
#include <utility>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "armor.h"
#include "general.h"

using namespace spdlog;

/**
 * The detect result of armor.
 * @param armor_type Type of armor.
 * @param corners_img Detected armor corners in image coord.
 * @param confidence Confidence of the prediction.
 */
struct DetectArmorInfo {
    ArmorID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float detection_confidence = 0.0;
};

struct ReconstructArmorInfo : DetectArmorInfo {

    explicit ReconstructArmorInfo() = default;

    explicit ReconstructArmorInfo(const DetectArmorInfo &det, const cv::Point3f &center_gimbal,
                                  const cv::Mat &rvec = cv::Mat(3, 1, CV_32F), float reconstruct_confidence = 1.0) {
        armor_type = det.armor_type;
        corners_img = det.corners_img;
        detection_confidence = det.detection_confidence;
        this->center_gimbal = center_gimbal;
        this->rvec = rvec;
        this->reconstruct_confidence = reconstruct_confidence;
    }

    cv::Point3f center_gimbal{};
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32F);
    float reconstruct_confidence = 0.0;
};

/**
 * The output of the Vision Armor detection.
 * @param seq_idx For test only. Index of the corresponding frame.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_info Detected armor info.
 */
struct DetectArmorResult {
    int seq_idx = -1;
    Time time{};
    std::vector<DetectArmorInfo> armor_info{};
};

struct ReconstructArmorResult : DetectArmorResult {

    explicit ReconstructArmorResult(const DetectArmorResult &det) {
        this->time = det.time;
        this->seq_idx = det.seq_idx;
    }

    std::vector<ReconstructArmorInfo> armor_info{};
};

/**
 * [DEPRECATED] The output of the Vision Part.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_type Type of armor.
 * @param corners_img_coord Detected armor corners in image coord.
 * @param confidence Confidence of the prediction.
 * @deprecated This struct is DEPRECATED. Use DetectArmorResult instead.
 */
struct ArmorPredResult {
    Time time{};
    ArmorID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float confidence{};
};

struct TrackArmorInfo {
    cv::Point3f center_gimbal;
    cv::Rect2f bbox;
};


#endif //CYGNOIDES_TYPING_GENERAL_H
