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
 */
struct DetectArmorInfo {

    // Info from vision detection part
    FacilityID facility_id = UNKNOWN_BOT;
    ArmorType armorType = SMALL_ARMOR;

    ArmorCorners2d corners_img;
    ArmorCorners3d corners_model;
    float detection_confidence = 1.0;

    // Info form reconstructor
    cv::Point3f center_base;
    cv::Point3f center_cam;
    Transform3d trans_model2cam;
    float reconstruct_confidence = 0.0;

    explicit DetectArmorInfo() = default;

    explicit DetectArmorInfo(FacilityID armor_type, const ArmorCorners2d &corners_img, float detection_confidence = 1.0) :
            facility_id(armor_type), corners_img(corners_img), detection_confidence(detection_confidence) {
        corners_model = ArmorCorners3d{getArmorTypeFormID(facility_id)};
    }
};

/**
 * The output of the Vision Armor detection.
 * @param seq_idx For test only. Index of the corresponding frame.
 * @param time The time **the frame is shot**. NOT THE TIME FINISH THE VISION PROCESS.
 * @param armor_info Detected armor info.
 */
struct DetectArmorsFrame {
    int seq_idx = -1;
    Time time{};
    std::vector<DetectArmorInfo> armor_info{};
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
    FacilityID armor_type = UNKNOWN;
    ArmorCorners2d corners_img;
    float confidence{};
};

struct TrackArmorInfo {
    cv::Point3f center_gimbal;
    cv::Rect2f bbox;
};


#endif //CYGNOIDES_TYPING_GENERAL_H
