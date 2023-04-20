//
// Created by catslashbin on 23-1-30.
//

#include "track_kalman.h"

using namespace cv;

TrackKalman::TrackKalman(Config &cfg, const ArmorInfo &detection, int frame_seq)
        : last_frame_seq_(frame_seq) {

    // Load params
    SC = cfg.get<float>("kalman.processNoiseCov.SC", 1e-2);
    SCM = cfg.get<float>("kalman.measurementNoiseCov.SCM", 0.01);
    dt = cfg.get<float>("kalman.processInterval", 0.01);

    // Init states

    Mat init_state;
    vconcat(cvtDetection2Measurement(detection), Mat::zeros(3, 1, CV_32F), init_state);
    kf.statePost = init_state;

/* clang-format off */

    kf.errorCovPost = (cv::Mat_<float>(6, 6)
            <<
            1e2, 0, 0, 0, 0, 0,
            0, 1e2, 0, 0, 0, 0,
            0, 0, 1e2, 0, 0, 0,
            0, 0, 0, 1e4, 0, 0,
            0, 0, 0, 0, 1e4, 0,
            0, 0, 0, 0, 0, 1e4);

    // Init matrices

    kf.measurementMatrix = cv::Mat::eye(3, 6, CV_32F);

    kf.transitionMatrix = (cv::Mat_<float>(6, 6)
            <<
            1, 0, 0, dt, 0, 0,
            0, 1, 0, 0, dt, 0,
            0, 0, 1, 0, 0, dt,
            0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1);

    float dt3 = dt * dt * dt, dt4 = dt * dt * dt * dt;
    kf.processNoiseCov = (cv::Mat_<float>(6, 6)
            <<
            0.25 * SC * dt4, 0, 0, 0.5 * SC * dt3, 0, 0,
            0, 0.25 * SC * dt4, 0, 0, 0.5 * SC * dt3, 0,
            0, 0, 0.25 * SC * dt4, 0, 0, 0.5 * SC * dt3,
            0.5 * SC * dt3, 0, 0, dt, 0, 0,
            0, 0.5 * SC * dt3, 0, 0, dt, 0,
            0, 0, 0.5 * SC * dt3, 0, 0, dt);

    kf.measurementNoiseCov = (cv::Mat_<float>(3, 3)
            <<
            SCM, 0, 0,
            0, SCM, 0,
            0, 0, SCM);

    // Matrix elements
    // SC, 0, 0,  0, 0, 0,  0.5 * SC * dt3, 0, 0,  0.25 * SC * dt4, 0, 0,
    // 0, SC, 0,  0, 0, 0,  0, 0.5 * SC * dt3, 0,  0, 0.25 * SC * dt4, 0,
    // 0, 0, SC,  0, 0, 0,  0, 0, 0.5 * SC * dt3,  0, 0, 0.25 * SC * dt4,

/* clang-format on */

    assert(kf.errorCovPost.rows == NUM_STATE * 2 && kf.errorCovPost.cols == NUM_STATE * 2);
    assert(kf.measurementMatrix.rows == NUM_STATE && kf.measurementMatrix.cols == NUM_STATE * 2);
    assert(kf.transitionMatrix.rows == NUM_STATE * 2 && kf.transitionMatrix.cols == NUM_STATE * 2);
    assert(kf.processNoiseCov.rows == NUM_STATE * 2 && kf.processNoiseCov.cols == NUM_STATE * 2);
    assert(kf.measurementNoiseCov.rows == NUM_STATE && kf.measurementNoiseCov.cols == NUM_STATE);

}

void TrackKalman::correct(const ArmorInfo &detection, int frame_seq) {
    for (int i = 0; i < frame_seq - last_frame_seq_; ++i) {
        kf.predict();
    }
    kf.correct(cvtDetection2Measurement(detection));
    last_frame_seq_ = frame_seq;
    debug("posConv {:.4f}, velConv {:.4f}, vel {}",
         kf.errorCovPost.at<float>(0, 0),
         kf.errorCovPost.at<float>(3, 3),
         kf.statePost.at<float>(3));
}

TrackArmorInfo TrackKalman::predict(int frame_seq) {
    return predict(frame_seq, 0.0f);
}

TrackArmorInfo TrackKalman::predict(int frame_seq, float pred_secs) {
    Mat state = kf.statePost.clone();
    for (int i = 0; i < NUM_STATE; ++i) {
        state.at<float>(i) += state.at<float>(i + NUM_STATE) * (float(frame_seq - last_frame_seq_) * dt + pred_secs);
    }
    return cvtStateMat2Result(state);
}

cv::Mat TrackKalman::cvtDetection2Measurement(const ArmorInfo &detection) {
    Point3f center = detection.target_world;
    return (Mat_<float>(3, 1) << center.x, center.y, center.z);
}

TrackArmorInfo TrackKalman::cvtStateMat2Result(const Mat &state) {

    float x = state.at<float>(STATE_X);
    float y = state.at<float>(STATE_Y);
    float z = state.at<float>(STATE_Z);

    return {Point3f{x, y, z}};
}