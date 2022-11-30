//
// Created by catslashbin on 22-11-21.
//

#include "tracked_armor.h"

class KalmanFilterUtils
{
public:
    static Mat getTransitionMatrix(float dt)
    {
        Mat transitionMatrix = (Mat_<float>(8, 8)
                <<
                1, 0, 0, 0, dt, 0, 0, 0,
                0, 1, 0, 0, 0, dt, 0, 0,
                0, 0, 1, 0, 0, 0, dt, 0,
                0, 0, 0, 1, 0, 0, 0, dt,
                0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 1
        );
        return transitionMatrix;
    }

    static Mat getMeasurementMat()
    {
        return Mat::eye(8, 4, CV_32F);
    }

    static Mat getProcessNoiseCov(float dt, float SD, float SR, float SH)
    {
        Mat processNoiseCov = (Mat_<float>(8, 8)
                <<
                SD * dt * dt, 0, 0, 0, SD * dt, 0, 0, 0,
                0, SD * dt * dt, 0, 0, 0, SD * dt, 0, 0,
                0, 0, SR * dt * dt, 0, 0, 0, SR * dt, 0,
                0, 0, 0, SH * dt * dt, 0, 0, 0, SH * dt,
                0, 0, 0, 0, SD, 0, 0, 0,
                0, 0, 0, 0, 0, SD, 0, 0,
                0, 0, 0, 0, 0, 0, SR, 0,
                0, 0, 0, 0, 0, 0, 0, SH
        );
        return processNoiseCov;
    }

    static Mat getMeasurementNoiseCov(float dt, float SDM, float SRM, float SHM)
    {
        Mat measurementNoiseCov = (Mat_<float>(4, 4)
                <<
                SDM, 0, 0, 0,
                0, SDM, 0, 0,
                0, 0, SRM, 0,
                0, 0, 0, SHM
        );
        return measurementNoiseCov;
    }

};

void TrackedArmor::correct(const ArmorInfo &armor, float dt)
{
    kf.transitionMatrix = KalmanFilterUtils::getTransitionMatrix(dt);
    // kf.measurementMatrix = KalmanFilterUtils::getMeasurementMat();
    kf.processNoiseCov = KalmanFilterUtils::getProcessNoiseCov(dt, 0.1, 0.1, 0.1);
    kf.measurementNoiseCov = KalmanFilterUtils::getMeasurementNoiseCov(dt, 0.1, 0.1, 0.1);

    auto center = armor.corners_img.center();
    float u = center.x, v = center.y;
    float ratio = armor.corners_img.ratio();
    float size = armor.corners_img.area();

    kf.correct(Mat({u, v, ratio, size}));
}

TrackedArmor::TrackedArmor()
{

}
