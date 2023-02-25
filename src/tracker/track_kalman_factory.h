//
// Created by catslashbin on 23-1-30.
//

#ifndef CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H
#define CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H

#include "rmdecis_impl/config_impl.h"

class TrackKalmanFactory {

    float SD, SR, SH, SC;
    float SDM, SRM, SHM, SCM;

public:

    explicit TrackKalmanFactory(Config& cfg);

    static cv::Mat getTransitionMatrix(float dt);

    static cv::Mat getMeasurementMat();

    cv::Mat getProcessNoiseCov(float dt) const;

    cv::Mat getMeasurementNoiseCov(float dt) const;

    static cv::Mat getInitState(const ArmorInfo &detection);

    static cv::Mat getInitError();
};


#endif //CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H
