//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_TRACKED_ARMOR_H
#define CYGNOIDES_DECISION_TRACKED_ARMOR_H

#include <opencv2/video/tracking.hpp>

#include "../typing/general.h"
#include "../typing/armor.h"

enum TrackingState
{
    TRACKING = 0,
    TENTATIVE = 1,
    LOST = 2
};

class TrackedArmor
{
    int tracked_T = 0, tentative_T = 0, lost_T = 0;

    KalmanFilter kf;
    std::array<int, 6> id_cnt{};

    TrackedArmor();
    void correct(const ArmorInfo& armor, float dt);
};


#endif //CYGNOIDES_DECISION_TRACKED_ARMOR_H
