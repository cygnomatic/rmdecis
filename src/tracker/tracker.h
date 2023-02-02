//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKER_H
#define CYGNOIDES_DECISION_TRACKER_H

#include <map>

#include "../../include/typing.h"
#include "reconstructor/camera_calib.h"
#include "munkres-cpp/munkres.h"
#include "armor_track.h"
#include "utils/config.h"

class Tracker {
private:

    TrackKalmanFactory kf_factory;

    std::map<int, ArmorTrack> armor_tracks_;
    int curr_id_ = 0;

    float k_similarity_threshold;
    int k_max_missing_cnt;
    int k_min_hit_cnt;

public:


    explicit Tracker(ConfigLoader &config_loader) : kf_factory(config_loader) {
        Config cfg = config_loader.load("tracker");
        k_similarity_threshold = cfg.get<float>("similarityThreshold", 0.7);
        k_max_missing_cnt = cfg.get<int>("maxMissingCount", 2);
        k_min_hit_cnt = cfg.get<int>("minHitCount", 4);
    }

    void update(const DetectArmorsFrame &reconstruct_armor_result);

    void associate(const std::vector<DetectArmorInfo> &armor_detections, Time time,
                   std::vector<DetectArmorInfo> &unmatched_detections,
                   std::map<int, DetectArmorInfo> &matched_track2det);

    std::map<int, ArmorTrack> getTracks(bool include_probationary = false);

};


#endif //CYGNOIDES_DECISION_TRACKER_H
