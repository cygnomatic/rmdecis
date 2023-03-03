//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKER_H
#define CYGNOIDES_DECISION_TRACKER_H

#include <map>

#include "rmdecis/core.h"

#include "reconstructor/camera_calib.h"
#include "tracker/armor_track.h"
#include "munkres-cpp/munkres.h"

class Tracker {
private:

    Config& cfg;

    std::map<int, ArmorTrack> armor_tracks_;
    int curr_id_ = 0;

    float k_similarity_threshold;
    int k_max_missing_cnt;
    int k_min_hit_cnt;

public:


    explicit Tracker(Config &cfg);

    void update(const std::vector<ArmorInfo> &armor_detections, int frame_seq);

    void associate(const std::vector<ArmorInfo> &armor_detections, int frame_seq,
                   std::vector<ArmorInfo> &unmatched_detections,
                   std::map<int, ArmorInfo> &matched_track2det);

    std::map<int, ArmorTrack> getTracks(bool include_probationary = false);
};


#endif //CYGNOIDES_DECISION_TRACKER_H
