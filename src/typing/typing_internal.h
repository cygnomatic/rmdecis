//
// Created by catslashbin on 23-2-25.
//

#ifndef CYGNOIDES_DECISION_TYPING_INTERNAL_H
#define CYGNOIDES_DECISION_TYPING_INTERNAL_H

#include "consts.h"
#include "typing.h"
#include "Eigen/Dense"

class Reconstructor;

struct CompetitionRule {

    // If STANDARD_3, STANDARD_4, STANDARD_5 using large armor
    bool is_standard_3_large;
    bool is_standard_4_large;
    bool is_standard_5_large;

    explicit CompetitionRule(Config &cfg);
};

struct ArmorInfo {

    // Info from vision detection part
    FacilityID facility_id = UNKNOWN_BOT;
    Corners2f corners_img;
    float detection_confidence = 1.0;

    // Info from reconstructor
    Corners3f corners_model;
    CvTransform3f trans_model2cam;
    float reconstruct_confidence = 0.0;
    cv::Point3f target_cam;
    cv::Point3f target_gimbal;
    cv::Point3f target_world;
    // WARNING: DO NOT DEL THE RECONSTRUCTOR! IT IS MAINTAINED IN THE BASIC_AIMING_IMPL!
    Reconstructor *reconstructor = nullptr;

    explicit ArmorInfo() = default;

    explicit ArmorInfo(DetectArmorInfo detect_armor_info, CompetitionRule &competition_rule);
};

struct VaneInfo {

    // Info from vision detection part
    bool is_target = true;
    Corners2f corners_img;
    float detection_confidence = 1.0;

    // Info from reconstructor
    Corners3f corners_model;
    CvTransform3f trans_model2cam;
    float reconstruct_confidence = 0.0;
    Eigen::Vector3f center_pt_cam, center_pt_world;
    Eigen::Vector3f target_pt_cam, target_pt_world;
    Eigen::Vector3f normal_pt_cam, normal_pt_world;

    explicit VaneInfo() = default;

    explicit VaneInfo(DetectVaneInfo detect_vane_info);
};


struct TrackArmorInfo {
    cv::Point3f target_world;
    cv::Rect2f bbox;
};
#endif //CYGNOIDES_DECISION_TYPING_INTERNAL_H
