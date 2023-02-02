//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACK_H
#define CYGNOIDES_DECISION_ARMOR_TRACK_H

#include <opencv2/video/tracking.hpp>
#include <numeric>

#include "../../include/typing.h"
#include "utils/cv_utils.h"
#include "tracker/track_kalman_factory.h"

#define NUM_STATE 7
enum StateMatDesc {
    STATE_U = 0,
    STATE_V = 1,
    STATE_RATIO = 2,
    STATE_AREA = 3,
    STATE_X = 4,
    STATE_Y = 5,
    STATE_Z = 6,
    STATE_D_U = 7,
    STATE_D_V = 8,
    STATE_D_RATIO = 9,
    STATE_D_AREA = 10,
    STATE_D_X = 11,
    STATE_D_Y = 12,
    STATE_D_Z = 13,
};

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

    static cv::Mat cvtDetection2MeasurementMat(const DetectArmorInfo &detection);

    static TrackArmorInfo cvtStateMat2Result(const cv::Mat &state);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACK_H
