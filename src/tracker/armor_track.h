//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACK_H
#define CYGNOIDES_DECISION_ARMOR_TRACK_H

#include <opencv2/video/tracking.hpp>
#include <numeric>

#include "../typing/general.h"
#include "../utils/cv_utils.h"

class ArmorTrack {
public:
    int tracking_id = -1;
    int missing_cnt = 0, hit_cnt = 0;

    ArmorTrack() = default;

    explicit ArmorTrack(int tracking_id, const DetectArmorInfo &armor);

    void init(const DetectArmorInfo &armor);

    void correct(const DetectArmorInfo &armor, Time time);

    cv::Rect2f predict(Time time);

    float calcSimilarity(const DetectArmorInfo &armor, Time time);

private:

    Time last_correct_time_;

    cv::KalmanFilter kf;

    std::array<int, NUM_ARMOR_ID> id_cnt{};

    float calcIdSimilarity(ArmorID id);

    void updateKalmanFilterMats(float dt);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACK_H
