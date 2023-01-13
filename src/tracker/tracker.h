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
    Time last_update_time;
    munkres::Munkres<float> munkres_f;

    std::map<int, ArmorTrack> armor_tracks;

    void update(DetectArmorResult detection);

    static munkres::Matrix<float> hungarianMatching(munkres::Matrix<float> mat);

    float SIMILARITY_THRESHOLD;

    std::vector<DetectArmorInfo> match(std::vector<DetectArmorInfo> &armor_detection, float dt);

    std::vector<DetectArmorInfo> match(const std::vector<DetectArmorInfo> &armor_detection, float dt);
};


#endif //CYGNOIDES_DECISION_TRACKER_H
