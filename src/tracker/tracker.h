//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKER_H
#define CYGNOIDES_DECISION_TRACKER_H

#include <map>

#include "../typing/general.h"
#include "../reconstructor/camera_calib.h"
#include "../contrib/munkres-cpp/munkres.h"
#include "armor_track.h"

class Tracker {
public:
    float k_similarity_threshold = 0.3;
    int k_max_missing_cnt = 3;
    int k_min_hit_cnt = 3;

    void update(const DetectArmorResult &detect_result);

    void associate(const std::vector<DetectArmorInfo> &armor_detections, float dt,
                   std::vector<DetectArmorInfo> &unmatched_detections,
                   std::map<int, DetectArmorInfo> &matched_track2det);

    std::map<int, ArmorTrack> getAllTracks();

private:
    std::map<int, ArmorTrack> armor_tracks_;
    Time last_update_time_;
    int curr_id_ = 0;
};


#endif //CYGNOIDES_DECISION_TRACKER_H
