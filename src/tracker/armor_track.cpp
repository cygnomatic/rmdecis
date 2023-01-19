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

    static inline Mat getTransitionMatrix(float dt) {
        Mat transitionMatrix = (Mat_<float>(14, 14)
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

    static inline Mat getMeasurementMat() {
        // State Matrix: [u, v, ratio, area, v_u, v_v, v_ration, v_area]
        return Mat::eye(7, 14, CV_32F);
    }

    static inline Mat getProcessNoiseCov(float dt) {
        float SD = 1e-1, SR = 1e-2, SH = 1e-2, SC = 1e-2; // ProcessNoise
        Mat processNoiseCov = (Mat_<float>(14, 14)
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

    static inline Mat getMeasurementNoiseCov(float dt) {
        float SDM = 100, SRM = 10, SHM = 10, SCM = 50; // MeasurementNoise
        Mat measurementNoiseCov = (Mat_<float>(7, 7)
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

    static inline Mat cvtDetection2MeasurementMat(const DetectArmorInfo &detection) {
        Rect2f bounding_box = (Rect2f) detection.corners_img;
        Point3f center = detection.center_gimbal;

        float u = bounding_box.x, v = bounding_box.y;
        float ratio = bounding_box.width / bounding_box.height;
        float area = bounding_box.area();

        // debug("Observation: u={}, v={}, r={}, a={}", u, v, ratio, area);

        return (Mat_<float>(7, 1) << u, v, ratio, area, center.x, center.y, center.z);
    };

    static inline TrackArmorInfo cvtStateMat2Result(const Mat &state) {
        auto pred_uv = Point2f{state.at<float>(STATE_U), state.at<float>(STATE_V)};

        // width^2 = (width * height) * (width / height) = area * ratio
        float width = sqrt(state.at<float>(STATE_AREA) * state.at<float>(STATE_RATIO));
        float height = width / state.at<float>(STATE_RATIO);

        float x = state.at<float>(STATE_X);
        float y = state.at<float>(STATE_Y);
        float z = state.at<float>(STATE_Z);

        // debug("State velocity: {} {} {} {}", state.at<float>(STATE_D_U), state.at<float>(STATE_D_V),
        //       state.at<float>(STATE_D_AREA), state.at<float>(STATE_D_RATIO));

        return {Point3f{x, y, z}, Rect2f{pred_uv, Size2f{width, height}}};
    };

    static inline Mat getInitState(const DetectArmorInfo &detection) {
        Rect2f bounding_box = detection.corners_img.getBoundingBox();
        Point3f center = detection.center_gimbal;


        float u = bounding_box.x, v = bounding_box.y;
        float ratio = bounding_box.width / bounding_box.height;
        float area = bounding_box.area();

        return (Mat_<float>(14, 1)
                <<
                u, v, ratio, area, center.x, center.y, center.z, 0, 0, 0, 0, 0, 0, 0);
    }

    static inline Mat getInitError() {
        return (Mat_<float>(14, 14)
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
};

ArmorTrack::ArmorTrack(int tracking_id, const DetectArmorInfo &detection) {
    this->tracking_id = tracking_id;
    init(detection);
}

void ArmorTrack::init(const DetectArmorInfo &detection) {

    kf = KalmanFilter(14, 7);

    // Init state
    kf.statePost = KalmanFilterFactory::getInitState(detection);
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


void ArmorTrack::correct(const DetectArmorInfo &detection, Time time) {
    updateKalmanFilterMats(time - last_correct_time_);
    last_correct_time_ = time;

    kf.predict(); // post(t-1, t-1) -> pre (t-1, t)
    kf.correct(KalmanFilterFactory::cvtDetection2MeasurementMat(detection)); // pre(t-1, t) -> post(t, t)

    id_cnt[detection.facility_id]++;
}

TrackArmorInfo ArmorTrack::predict(Time time) {
    updateKalmanFilterMats(time - last_correct_time_);

    // We need to call predict several times, so we
    // preserve original state to prevent them being updated multiple times.
    Mat oriStatePost = kf.statePost.clone();
    Mat oriErrorCovPost = kf.errorCovPost.clone();

    auto ret = KalmanFilterFactory::cvtStateMat2Result(kf.predict());

    kf.statePost = oriStatePost.clone();
    kf.errorCovPost = oriErrorCovPost.clone();

    return ret;
}

float ArmorTrack::calcSimilarity(const DetectArmorInfo &detection, Time time) {
    auto new_bounding_box = (Rect2f) detection.corners_img;
    auto pred_bounding_box = predict(time).bbox;

    float iou = calculateIoU(pred_bounding_box, new_bounding_box);
    float id_similarity = calcIdSimilarity(detection.facility_id);

    float ret = iou * 1.0 + id_similarity * 0.0;
    assert(!isnanf(ret));

    return ret;
}

float ArmorTrack::calcIdSimilarity(FacilityID id) {
    int sum = std::accumulate(id_cnt.begin(), id_cnt.end(), 2);
    return (float) (id_cnt[id] + 1) /
           (float) sum; // Here we artificially add one positive and one negative. To smoothen the result.
}

