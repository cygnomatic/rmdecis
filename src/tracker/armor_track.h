//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACK_H
#define CYGNOIDES_DECISION_ARMOR_TRACK_H

#include <opencv2/video/tracking.hpp>
#include <numeric>

#include "../typing/general.h"
#include "../utils/cv_utils.h"

class ArmorTrack
{
public:

    int tracking_id{};
    int tracked_T = 0, tentative_T = 0, lost_T = 0;

    ArmorTrack() = default;

    explicit ArmorTrack(int tracking_id, const ArmorInfo &armor);

    void init(const ArmorInfo &armor);

    void correct(const ArmorInfo &armor, float dt);

    Rect2f predict(float dt);

    float calcSimilarity(const DetectArmorInfo &armor, float dt);

private:

    KalmanFilter kf;

    std::array<int, NUM_ARMOR_ID> id_cnt{};

    float calcIdSimilarity(ArmorID id);

    void updateKalmanFilterMats(float dt);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACK_H
