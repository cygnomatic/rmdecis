//
// Created by catslashbin on 23-1-30.
//

#ifndef CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H
#define CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H

#include "utils/config.h"

class TrackKalmanFactory {

    float SD, SR, SH, SC;
    float SDM, SRM, SHM, SCM;

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

public:

    explicit TrackKalmanFactory(ConfigLoader config_loader);

    static cv::Mat getTransitionMatrix(float dt);

    static cv::Mat getMeasurementMat();

    cv::Mat getProcessNoiseCov(float dt);

    cv::Mat getMeasurementNoiseCov(float dt);

    static cv::Mat cvtDetection2MeasurementMat(const DetectArmorInfo &detection);;

    static TrackArmorInfo cvtStateMat2Result(const cv::Mat &state);;

    static cv::Mat getInitState(const DetectArmorInfo &detection);

    static cv::Mat getInitError();
};


#endif //CYGNOIDES_DECISION_TRACK_KALMAN_FACTORY_H
