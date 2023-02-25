//
// Created by catslashbin on 23-1-30.
//

#include "rmdecis_impl/config_impl.h"
#include "track_kalman_factory.h"
#include "armor_track.h"
#include "typing_internal.h"

cv::Mat TrackKalmanFactory::getInitError() {
    return (cv::Mat_<float>(14, 14)
            <<
            100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000);
}

cv::Mat TrackKalmanFactory::getInitState(const DetectArmorInfo &detection) {
    cv::Rect2f bounding_box = detection.corners_img.getBoundingBox();
    cv::Point3f center = detection.target_world;


    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    return (cv::Mat_<float>(14, 1)
            <<
            u, v, ratio, area, center.x, center.y, center.z, 0, 0, 0, 0, 0, 0, 0);
}

cv::Mat TrackKalmanFactory::getMeasurementNoiseCov(float dt) const {
    cv::Mat measurementNoiseCov = (cv::Mat_<float>(7, 7)
            <<
            SDM, 0, 0, 0, 0, 0, 0,
            0, SDM, 0, 0, 0, 0, 0,
            0, 0, SRM, 0, 0, 0, 0,
            0, 0, 0, SHM, 0, 0, 0,
            0, 0, 0, 0, SCM, 0, 0,
            0, 0, 0, 0, 0, SCM, 0,
            0, 0, 0, 0, 0, 0, SCM
    );
    return measurementNoiseCov;
}

cv::Mat TrackKalmanFactory::getProcessNoiseCov(float dt) const {
    cv::Mat processNoiseCov = (cv::Mat_<float>(14, 14)
            <<
            SD * dt * dt, 0, 0, 0, 0, 0, 0, SD * dt, 0, 0, 0, 0, 0, 0,
            0, SD * dt * dt, 0, 0, 0, 0, 0, 0, SD * dt, 0, 0, 0, 0, 0,
            0, 0, SR * dt * dt, 0, 0, 0, 0, 0, 0, SR * dt, 0, 0, 0, 0,
            0, 0, 0, SH * dt * dt, 0, 0, 0, 0, 0, 0, SH * dt, 0, 0, 0,
            0, 0, 0, 0, SC * dt * dt, 0, 0, 0, 0, 0, 0, SC * dt, 0, 0,
            0, 0, 0, 0, 0, SC * dt * dt, 0, 0, 0, 0, 0, 0, SC * dt, 0,
            0, 0, 0, 0, 0, 0, SC * dt * dt, 0, 0, 0, 0, 0, 0, SC * dt,
            0, 0, 0, 0, 0, 0, 0, SD, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, SD, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, SR, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SH, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC
    );
    return processNoiseCov;
}

cv::Mat TrackKalmanFactory::getMeasurementMat() {
    // State Matrix: [u, v, ratio, area, v_u, v_v, v_ration, v_area]
    return cv::Mat::eye(7, 14, CV_32F);
}

cv::Mat TrackKalmanFactory::getTransitionMatrix(float dt) {
    cv::Mat transitionMatrix = (cv::Mat_<float>(14, 14)
            <<
            1, 0, 0, 0, 0, 0, 0, dt, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0, 0, dt, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0, 0, dt, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, dt, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, dt, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, dt, 0,
            0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, dt,
            0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    );
    return transitionMatrix;
}

TrackKalmanFactory::TrackKalmanFactory(Config& cfg) {

    SD = cfg.get<float>("kalman.processNoiseCov.SD", 1e-1);
    SR = cfg.get<float>("kalman.processNoiseCov.SR", 1e-2);
    SH = cfg.get<float>("kalman.processNoiseCov.SH", 1e-2);
    SC = cfg.get<float>("kalman.processNoiseCov.SC", 1e-2);

    SDM = cfg.get<float>("kalman.measurementNoiseCov.SDM", 100);
    SRM = cfg.get<float>("kalman.measurementNoiseCov.SRM", 10);
    SHM = cfg.get<float>("kalman.measurementNoiseCov.SHM", 10);
    SCM = cfg.get<float>("kalman.measurementNoiseCov.SCM", 50);
}
