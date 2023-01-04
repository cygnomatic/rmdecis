//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKER_H
#define CYGNOIDES_DECISION_TRACKER_H

#include "../typing/general.h"
#include "../reconstructor/camera_calib.h"
#include "armor_track.h"

class Tracker
{
    float last_t;
    std::vector<ArmorTrack> tracking_armors;

    void update(std::vector<ArmorPredResult>, float time);

    void predict(float time);

    std::vector<ArmorTrack> & get_all_tracking_armors();
};


#endif //CYGNOIDES_DECISION_TRACKER_H
