//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACK_H
#define CYGNOIDES_DECISION_ARMOR_TRACK_H

#include <opencv2/video/tracking.hpp>
#include <numeric>

#include "typing/core.h"
#include "utils/cv_utils.h"
#include "tracker/track_kalman_factory.h"

class ArmorTrack {
public:
    int tracking_id = -1;
    int missing_cnt = 0, hit_cnt = 0;

    explicit ArmorTrack(int tracking_id, const DetectArmorInfo &detection, const TrackKalmanFactory &factory);

    void init(const DetectArmorInfo &detection);

    void correct(const DetectArmorInfo &detection, Time time);

    TrackArmorInfo predict(Time time);

    float calcSimilarity(const DetectArmorInfo &detection, Time time);

private:

    TrackKalmanFactory factory;

    Time last_correct_time_;

    cv::KalmanFilter kf;

    std::array<int, MAX_FACILITY_ID> id_cnt{};

    float calcIdSimilarity(FacilityID id);

    void updateKalmanFilterMats(float dt);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACK_H
