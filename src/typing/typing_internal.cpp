//
// Created by catslashbin on 23-2-25.
//

#include "rmdecis/core.h"
#include "typing_internal.h"


ArmorInfo::ArmorInfo(DetectArmorInfo detect_armor_info) {

    facility_id = detect_armor_info.facility_id;
    corners_img = std::move(detect_armor_info.corners_img);
    detection_confidence = detect_armor_info.detection_confidence;

    corners_model = SmallRectCorners3f();
}

VaneInfo::VaneInfo(DetectVaneInfo detect_vane_info) {

    is_target = detect_vane_info.is_target;
    corners_img = std::move(detect_vane_info.corners_img);
    detection_confidence = detect_vane_info.detection_confidence;

    corners_model = RuneCorners3f();
}