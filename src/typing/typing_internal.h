//
// Created by catslashbin on 23-2-25.
//

#ifndef CYGNOIDES_DECISION_TYPING_INTERNAL_H
#define CYGNOIDES_DECISION_TYPING_INTERNAL_H

#include "consts.h"
#include "typing.h"

/**
 * The detect result of armor.
 */
struct ArmorInfo {

    // Info from vision detection part
    FacilityID facility_id = UNKNOWN_BOT;
    Corners3f corners_model;
    Corners2f corners_img;
    float detection_confidence = 1.0;

    // Info from reconstructor
    cv::Point3f target_cam;
    cv::Point3f target_gimbal;
    cv::Point3f target_world;

    CvTransform3f trans_model2cam;
    float reconstruct_confidence = 0.0;

    explicit ArmorInfo() = default;

    explicit ArmorInfo(DetectArmorInfo detect_armor_info);
};


struct TrackArmorInfo {
    cv::Point3f center_gimbal;
    cv::Rect2f bbox;
};
#endif //CYGNOIDES_DECISION_TYPING_INTERNAL_H
