//
// Created by catslashbin on 23-1-30.
//

#include "utils/config.h"
#include "track_kalman_factory.h"

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
    cv::Point3f center = detection.center_base;


    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    return (cv::Mat_<float>(14, 1)
            <<
            u, v, ratio, area, center.x, center.y, center.z, 0, 0, 0, 0, 0, 0, 0);
}

TrackArmorInfo TrackKalmanFactory::cvtStateMat2Result(const cv::Mat &state) {
    auto pred_uv = cv::Point2f{state.at<float>(STATE_U), state.at<float>(STATE_V)};

    // width^2 = (width * height) * (width / height) = area * ratio
    float width = std::sqrt(state.at<float>(STATE_AREA) * state.at<float>(STATE_RATIO));
    float height = width / state.at<float>(STATE_RATIO);

    float x = state.at<float>(STATE_X);
    float y = state.at<float>(STATE_Y);
    float z = state.at<float>(STATE_Z);

    // debug("State velocity: {} {} {} {}", state.at<float>(STATE_D_U), state.at<float>(STATE_D_V),
    //       state.at<float>(STATE_D_AREA), state.at<float>(STATE_D_RATIO));

    return {cv::Point3f{x, y, z}, cv::Rect2f{pred_uv, cv::Size2f{width, height}}};
}

cv::Mat TrackKalmanFactory::cvtDetection2MeasurementMat(const DetectArmorInfo &detection) {
    cv::Rect2f bounding_box = (cv::Rect2f) detection.corners_img;
    cv::Point3f center = detection.center_base;

    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    // debug("Observation: u={}, v={}, r={}, a={}", u, v, ratio, area);

    return (cv::Mat_<float>(7, 1) << u, v, ratio, area, center.x, center.y, center.z);
}

cv::Mat TrackKalmanFactory::getMeasurementNoiseCov(float dt) {
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

cv::Mat TrackKalmanFactory::getProcessNoiseCov(float dt) {
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

TrackKalmanFactory::TrackKalmanFactory(ConfigLoader config_loader) {
    Config cfg = config_loader.load("kalman");

    SD = cfg.get<float>("processNoiseCov.SD", 1e-1);
    SR = cfg.get<float>("processNoiseCov.SR", 1e-2);
    SH = cfg.get<float>("processNoiseCov.SH", 1e-2);
    SC = cfg.get<float>("processNoiseCov.SC", 1e-2);

    SDM = cfg.get<float>("processNoiseCov.SDM", 100);
    SRM = cfg.get<float>("processNoiseCov.SRM", 10);
    SHM = cfg.get<float>("processNoiseCov.SHM", 10);
    SCM = cfg.get<float>("processNoiseCov.SCM", 50);
}
