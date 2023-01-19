//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKER_H
#define CYGNOIDES_DECISION_TRACKER_H

#include <map>

#include "typing/core.h"
#include "reconstructor/camera_calib.h"
#include "munkres-cpp/munkres.h"
#include "armor_track.h"

class Tracker {
public:
    float k_similarity_threshold = 0.7;
    int k_max_missing_cnt = 2;
    int k_min_hit_cnt = 4;

    // Tracker() = default;

    explicit Tracker(float k_similarity_threshold = 0.7, int k_max_missing_cnt = 2, int k_min_hit_cnt = 4) :
            k_similarity_threshold(k_similarity_threshold), k_max_missing_cnt(k_max_missing_cnt),
            k_min_hit_cnt(k_min_hit_cnt) {}

    void update(const DetectArmorsFrame &reconstruct_armor_result);

    void associate(const std::vector<DetectArmorInfo> &armor_detections, Time time,
                   std::vector<DetectArmorInfo> &unmatched_detections,
                   std::map<int, DetectArmorInfo> &matched_track2det);

    std::map<int, ArmorTrack> getTracks(bool include_probationary = false);

private:
    std::map<int, ArmorTrack> armor_tracks_;
    int curr_id_ = 0;
};


#endif //CYGNOIDES_DECISION_TRACKER_H
