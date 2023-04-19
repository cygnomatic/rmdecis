//
// Created by catslashbin on 23-1-30.
//

#ifndef CYGNOIDES_DECISION_TRACK_KALMAN_H
#define CYGNOIDES_DECISION_TRACK_KALMAN_H

#include "rmdecis/core.h"

#define NUM_STATE 3
enum StateMatDesc {
    STATE_X = 0,
    STATE_Y = 1,
    STATE_Z = 2,
    STATE_D_X = 3,
    STATE_D_Y = 4,
    STATE_D_Z = 5,
};


class TrackKalman {
public:

    cv::KalmanFilter kf = cv::KalmanFilter(6, 3);

public:

    TrackKalman(Config &cfg, const ArmorInfo &detection, int frame_seq);

    static cv::Mat cvtDetection2Measurement(const ArmorInfo &detection);

    static TrackArmorInfo cvtStateMat2Result(const cv::Mat &state);

    void correct(const ArmorInfo &detection, int frame_seq);

    TrackArmorInfo predict(int frame_seq);

    TrackArmorInfo predict(int frame_seq, float pred_secs);

private:

    float SC; // Process noise

    float SCM; // Measurement noise

    float dt; // Process interval

    int last_frame_seq_;
};


#endif //CYGNOIDES_DECISION_TRACK_KALMAN_H
