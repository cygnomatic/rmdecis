//
// Created by catslashbin on 23-1-30.
//

#ifndef CYGNOIDES_DECISION_TRACK_KALMAN_H
#define CYGNOIDES_DECISION_TRACK_KALMAN_H

#include "rmdecis/core.h"

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


class TrackKalman {
public:

    cv::KalmanFilter kf = cv::KalmanFilter(14, 7);

public:

    TrackKalman(Config &cfg, const ArmorInfo &detection, int frame_seq);

    static cv::Mat cvtDetection2Measurement(const ArmorInfo &detection);

    static TrackArmorInfo cvtStateMat2Result(const cv::Mat &state);

    void correct(const ArmorInfo &detection, int frame_seq);

    TrackArmorInfo predict(int frame_seq);

private:

    // Process noise
    float SD, SR, SH, SC;

    // Measurement noise
    float SDM, SRM, SHM, SCM;

    int last_frame_seq_;

    const float dt = 0.015;
};


#endif //CYGNOIDES_DECISION_TRACK_KALMAN_H
