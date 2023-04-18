//
// Created by catslashbin on 23-1-30.
//

#include "track_kalman.h"

using namespace cv;

TrackKalman::TrackKalman(Config &cfg, const ArmorInfo &detection, int frame_seq)
        : last_frame_seq_(frame_seq) {

    // Load params
    SD = cfg.get<float>("kalman.processNoiseCov.SD", 1e-1);
    SR = cfg.get<float>("kalman.processNoiseCov.SR", 1e-2);
    SH = cfg.get<float>("kalman.processNoiseCov.SH", 1e-2);
    SC = cfg.get<float>("kalman.processNoiseCov.SC", 1e-2);

    SDM = cfg.get<float>("kalman.measurementNoiseCov.SDM", 100);
    SRM = cfg.get<float>("kalman.measurementNoiseCov.SRM", 10);
    SHM = cfg.get<float>("kalman.measurementNoiseCov.SHM", 10);
    SCM = cfg.get<float>("kalman.measurementNoiseCov.SCM", 50);

    dt = cfg.get<float>("kalman.processInterval", 0.01);

    // Init states

    Mat init_state;
    vconcat(cvtDetection2Measurement(detection), Mat::zeros(7, 1, CV_32F), init_state);
    kf.statePost = init_state;

// clang-format off

    kf.errorCovPost = (cv::Mat_<float>(14, 14)
            <<
            1e2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1e2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1e2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1e2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 1e2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 1e2, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1e2, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 1e4, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1e4, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1e4, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e4, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e4, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e4, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e4);

    // Init matrices

    kf.measurementMatrix = cv::Mat::eye(7, 14, CV_32F);

    kf.transitionMatrix = (cv::Mat_<float>(14, 14)
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
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

    kf.processNoiseCov = (cv::Mat_<float>(14, 14)
            <<
            SD, 0, 0, 0, 0, 0, 0, SD * dt, 0, 0, 0, 0, 0, 0,
            0, SD, 0, 0, 0, 0, 0, 0, SD * dt, 0, 0, 0, 0, 0,
            0, 0, SR, 0, 0, 0, 0, 0, 0, SR * dt, 0, 0, 0, 0,
            0, 0, 0, SH, 0, 0, 0, 0, 0, 0, SH * dt, 0, 0, 0,
            0, 0, 0, 0, SC, 0, 0, 0, 0, 0, 0, SC * dt, 0, 0,
            0, 0, 0, 0, 0, SC, 0, 0, 0, 0, 0, 0, SC * dt, 0,
            0, 0, 0, 0, 0, 0, SC, 0, 0, 0, 0, 0, 0, SC * dt,
            SD * dt, 0, 0, 0, 0, 0, 0, SD * dt * dt, 0, 0, 0, 0, 0, 0,
            0, SD * dt, 0, 0, 0, 0, 0, 0, SD * dt * dt, 0, 0, 0, 0, 0,
            0, 0, SR * dt, 0, 0, 0, 0, 0, 0, SR * dt * dt, 0, 0, 0, 0,
            0, 0, 0, SH * dt, 0, 0, 0, 0, 0, 0, SH * dt * dt, 0, 0, 0,
            0, 0, 0, 0, SC * dt, 0, 0, 0, 0, 0, 0, SC * dt * dt, 0, 0,
            0, 0, 0, 0, 0, SC * dt, 0, 0, 0, 0, 0, 0, SC * dt * dt, 0,
            0, 0, 0, 0, 0, 0, SC * dt, 0, 0, 0, 0, 0, 0, SC * dt * dt);

    kf.measurementNoiseCov = (cv::Mat_<float>(7, 7)
            <<
            SDM, 0, 0, 0, 0, 0, 0,
            0, SDM, 0, 0, 0, 0, 0,
            0, 0, SRM, 0, 0, 0, 0,
            0, 0, 0, SHM, 0, 0, 0,
            0, 0, 0, 0, SCM, 0, 0,
            0, 0, 0, 0, 0, SCM, 0,
            0, 0, 0, 0, 0, 0, SCM);

// clang-format on

}

void TrackKalman::correct(const ArmorInfo &detection, int frame_seq) {
    for (int i = 0; i < frame_seq - last_frame_seq_; ++i) {
        kf.predict();
    }
    kf.correct(cvtDetection2Measurement(detection));
    last_frame_seq_ = frame_seq;
}

TrackArmorInfo TrackKalman::predict(int frame_seq) {
    Mat state = kf.statePost.clone();
    for (int i = 0; i < NUM_STATE; ++i) {
        state.at<float>(i) += state.at<float>(i + NUM_STATE) * float(frame_seq - last_frame_seq_) * dt;
    }
    return cvtStateMat2Result(state);
}

TrackArmorInfo TrackKalman::predict(int frame_seq, float pred_secs) {
    return predict(frame_seq + int(pred_secs / dt));
}

cv::Mat TrackKalman::cvtDetection2Measurement(const ArmorInfo &detection) {
    Rect2f bounding_box = detection.corners_img.getBoundingBox();
    Point3f center = detection.target_world;

    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    return (Mat_<float>(7, 1) << u, v, ratio, area, center.x, center.y, center.z);
}

TrackArmorInfo TrackKalman::cvtStateMat2Result(const Mat &state) {
    auto pred_uv = Point2f{state.at<float>(STATE_U), state.at<float>(STATE_V)};

    // width^2 = (width * height) * (width / height) = area * ratio
    float width = std::sqrt(state.at<float>(STATE_AREA) * state.at<float>(STATE_RATIO));
    float height = width / state.at<float>(STATE_RATIO);

    float x = state.at<float>(STATE_X);
    float y = state.at<float>(STATE_Y);
    float z = state.at<float>(STATE_Z);

    return {Point3f{x, y, z}, Rect2f{pred_uv, Size2f{width, height}}};
}