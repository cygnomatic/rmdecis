//
// Created by catslashbin on 22-11-21.
//

#include "armor_track.h"

using namespace cv;

class KalmanFilterFactory {
public:
    enum StateMatDesc {
        STATE_U = 0,
        STATE_V = 1,
        STATE_RATIO = 2,
        STATE_AREA = 3,
        STATE_D_U = 4,
        STATE_D_V = 5,
        STATE_D_RATIO = 6,
        STATE_D_AREA = 7
    };

    static inline Mat getTransitionMatrix(float dt) {
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

    static inline Mat getMeasurementMat() {
        // State Matrix: [u, v, ratio, area, v_u, v_v, v_ration, v_area]
        return Mat::eye(4, 8, CV_32F);
    }

    static inline Mat getProcessNoiseCov(float dt) {
        float SD = 1e-1, SR = 1e-2, SH = 1e-2; // ProcessNoise
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

    static inline Mat getMeasurementNoiseCov(float dt) {
        float SDM = 2, SRM = 1, SHM = 1; // MeasurementNoise
        Mat measurementNoiseCov = (Mat_<float>(4, 4)
                <<
                SDM, 0, 0, 0,
                0, SDM, 0, 0,
                0, 0, SRM, 0,
                0, 0, 0, SHM
        );
        return measurementNoiseCov;
    }

    static inline Mat cvtCorners2MeasurementMat(const ArmorCorners2d &corners) {
        Rect2f bounding_box = (Rect2f) corners;

        float u = bounding_box.x, v = bounding_box.y;
        float ratio = bounding_box.width / bounding_box.height;
        float area = bounding_box.area();

        return (Mat_<float>(4, 1) << u, v, ratio, area);
    };

    static inline Rect2f cvtStateMat2Rect(const Mat &state) {
        auto pred_uv = Point2f{state.at<float>(STATE_U), state.at<float>(STATE_V)};

        // width^2 = (width * height) * (width / height) = area * ratio
        float width = sqrt(state.at<float>(STATE_AREA) * state.at<float>(STATE_RATIO));
        float height = width / state.at<float>(STATE_RATIO);

        return {pred_uv, Size2f{width, height}};
    };

    static inline Mat getInitState(const ArmorCorners2d &corners) {
        Rect2f bounding_box = (Rect2f) corners;

        float u = bounding_box.x, v = bounding_box.y;
        float ratio = bounding_box.width / bounding_box.height;
        float area = bounding_box.area();

        return (Mat_<float>(8, 1) << u, v, ratio, area, 0, 0, 0, 0);
    }

    static inline Mat getInitError() {
        return (Mat_<float>(8, 8) <<
              100, 0, 0, 0, 0, 0, 0, 0,
                0, 100, 0, 0, 0, 0, 0, 0,
                0, 0, 100, 0, 0, 0, 0, 0,
                0, 0, 0, 100, 0, 0, 0, 0,
                0, 0, 0, 0, 10000, 0, 0, 0,
                0, 0, 0, 0, 0, 10000, 0, 0,
                0, 0, 0, 0, 0, 0, 10000, 0,
                0, 0, 0, 0, 0, 0, 0, 10000);
    }
};

ArmorTrack::ArmorTrack(int tracking_id, const DetectArmorInfo &armor) {
    this->tracking_id = tracking_id;
    init(armor);
}

void ArmorTrack::init(const DetectArmorInfo &armor) {

    kf = KalmanFilter(8, 4);

    // Init state
    kf.statePost = KalmanFilterFactory::getInitState(armor.corners_img);
    kf.errorCovPost = KalmanFilterFactory::getInitError();

    // Init mats
    kf.measurementMatrix = KalmanFilterFactory::getMeasurementMat();
    updateKalmanFilterMats(0.1);
}

void ArmorTrack::updateKalmanFilterMats(float dt) {
    kf.transitionMatrix = KalmanFilterFactory::getTransitionMatrix(dt);
    kf.processNoiseCov = KalmanFilterFactory::getProcessNoiseCov(dt);
    kf.measurementNoiseCov = KalmanFilterFactory::getMeasurementNoiseCov(dt);
}


void ArmorTrack::correct(const DetectArmorInfo &armor, Time time) {
    updateKalmanFilterMats(time - last_correct_time_);
    last_correct_time_ = time;

    kf.predict(); // post(t-1, t-1) -> pre (t-1, t)
    kf.correct(KalmanFilterFactory::cvtCorners2MeasurementMat(armor.corners_img)); // pre(t-1, t) -> post(t, t)

    id_cnt[armor.armor_type]++;
}

Rect2f ArmorTrack::predict(Time time) {
    updateKalmanFilterMats(time - last_correct_time_);
    Rect2f ret;

    // We need to call predict several times, so we
    // preserve original state to prevent them being updated multiple times.
    Mat oriStatePost = kf.statePost.clone();
    Mat oriErrorCovPost = kf.errorCovPost.clone();

    ret = KalmanFilterFactory::cvtStateMat2Rect(kf.predict());

    kf.statePost = oriStatePost.clone();
    kf.errorCovPost = oriErrorCovPost.clone();

    return ret;
}

float ArmorTrack::calcSimilarity(const DetectArmorInfo &armor, Time time) {
    auto new_bounding_box = (Rect2f) armor.corners_img;
    auto pred_bounding_box = predict(time);

    float iou = calculateIoU(pred_bounding_box, new_bounding_box);
    float id_similarity = calcIdSimilarity(armor.armor_type);

    float ret = iou * 1.0 + id_similarity * 0.0;
    assert(!isnanf(ret));

    return ret;
}

float ArmorTrack::calcIdSimilarity(ArmorID id) {
    int sum = std::accumulate(id_cnt.begin(), id_cnt.end(), 2);
    return (float) (id_cnt[id] + 1) /
           (float) sum; // Here we artificially add one positive and one negative. To smoothen the result.
}

