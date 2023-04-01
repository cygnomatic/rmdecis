//
// Created by catslashbin on 23-3-31.
//

#ifndef CYGNOIDES_DECISION_PREPROCESS_H
#define CYGNOIDES_DECISION_PREPROCESS_H

#include "typing.h"

inline std::vector<ArmorInfo> detectionToInfo(const std::vector<DetectArmorInfo> &detect_armor_infos,
                                              int k_frame_width, int k_frame_height, float k_confidence_threshold,
                                              CompetitionRule competition_rule) {

    auto armor_info = std::vector<ArmorInfo>();
    for (const auto &a: detect_armor_infos) {
        cv::Rect2i bbox = a.corners_img.getBoundingBox();

        // Skip detections outside the screen
        if (bbox.x <= 0 || bbox.x + bbox.width >= k_frame_width
            || bbox.y <= 0 || bbox.y + bbox.height >= k_frame_height)
            continue;

        // Skip detections with low confidence
        if (a.detection_confidence < k_confidence_threshold)
            continue;

        armor_info.emplace_back(a, competition_rule);
    }

    return armor_info;
}

#endif //CYGNOIDES_DECISION_PREPROCESS_H
