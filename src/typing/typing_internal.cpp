//
// Created by catslashbin on 23-2-25.
//

#include "rmdecis/core.h"
#include "typing_internal.h"

CompetitionRule::CompetitionRule(Config &cfg) :
        is_standard_3_large(cfg.get<bool>("globalRule.isStandard3Large")),
        is_standard_4_large(cfg.get<bool>("globalRule.isStandard4Large")),
        is_standard_5_large(cfg.get<bool>("globalRule.isStandard3Large")) {}

ArmorInfo::ArmorInfo(DetectArmorInfo detect_armor_info, CompetitionRule &rule) {

    facility_id = detect_armor_info.facility_id;
    corners_img = std::move(detect_armor_info.corners_img);
    detection_confidence = detect_armor_info.detection_confidence;

    if ((facility_id == STANDARD_3 && rule.is_standard_3_large) ||
        (facility_id == STANDARD_4 && rule.is_standard_4_large) ||
        (facility_id == STANDARD_5 && rule.is_standard_5_large) ||
        facility_id == HERO_1 || facility_id == BASE_8)
        corners_model = LargeRectCorners3f();
    else
        corners_model = SmallRectCorners3f();
}

VaneInfo::VaneInfo(DetectVaneInfo detect_vane_info) {

    is_target = detect_vane_info.is_target;
    corners_img = std::move(detect_vane_info.corners_img);
    detection_confidence = detect_vane_info.detection_confidence;

    corners_model = RuneCorners3f();
}
