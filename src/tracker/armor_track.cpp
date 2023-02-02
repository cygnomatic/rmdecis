//
// Created by catslashbin on 22-11-21.
//

#include "armor_track.h"
#include "track_kalman_factory.h"
#include "rmdecis/core.h"

using namespace cv;

ArmorTrack::ArmorTrack(int tracking_id, const DetectArmorInfo &detection, const TrackKalmanFactory &factory)
        : factory(factory), tracking_id(tracking_id) {
    init(detection);
}

void ArmorTrack::init(const DetectArmorInfo &detection) {

    kf = KalmanFilter(14, 7);

    // Init state
    kf.statePost = factory.getInitState(detection);
    kf.errorCovPost = factory.getInitError();

    // Init mats
    kf.measurementMatrix = factory.getMeasurementMat();
    updateKalmanFilterMats(0.1);
}

void ArmorTrack::updateKalmanFilterMats(float dt) {
    kf.transitionMatrix = factory.getTransitionMatrix(dt);
    kf.processNoiseCov = factory.getProcessNoiseCov(dt);
    kf.measurementNoiseCov = factory.getMeasurementNoiseCov(dt);
}


void ArmorTrack::correct(const DetectArmorInfo &detection, Time time) {
    updateKalmanFilterMats(time - last_correct_time_);
    last_correct_time_ = time;

    kf.predict(); // post(t-1, t-1) -> pre (t-1, t)
    kf.correct(cvtDetection2MeasurementMat(detection)); // pre(t-1, t) -> post(t, t)

    id_cnt[detection.facility_id]++;
}

TrackArmorInfo ArmorTrack::predict(Time time) {

    // updateKalmanFilterMats(time - last_correct_time_);
    //
    // // We need to call predict several times, so we
    // // preserve original state to prevent them being updated multiple times.
    // Mat oriStatePost = kf.statePost.clone();
    // Mat oriErrorCovPost = kf.errorCovPost.clone();
    //
    // auto ret = cvtStateMat2Result(kf.predict());
    //
    // kf.statePost = oriStatePost.clone();
    // kf.errorCovPost = oriErrorCovPost.clone();

    // We use const velocity model, so pred_state = state + t * velocity.

    Mat state = kf.statePost.clone();
    for (int i = 0; i < NUM_STATE; ++i) {
        state.at<float>(i) += state.at<float>(i + NUM_STATE) * (time - last_correct_time_);
    }

    return cvtStateMat2Result(state);
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

TrackArmorInfo ArmorTrack::cvtStateMat2Result(const Mat &state) {
    auto pred_uv = Point2f{state.at<float>(STATE_U), state.at<float>(STATE_V)};

    // width^2 = (width * height) * (width / height) = area * ratio
    float width = std::sqrt(state.at<float>(STATE_AREA) * state.at<float>(STATE_RATIO));
    float height = width / state.at<float>(STATE_RATIO);

    float x = state.at<float>(STATE_X);
    float y = state.at<float>(STATE_Y);
    float z = state.at<float>(STATE_Z);

    return {Point3f{x, y, z}, Rect2f{pred_uv, Size2f{width, height}}};
}

Mat ArmorTrack::cvtDetection2MeasurementMat(const DetectArmorInfo &detection) {
    Rect2f bounding_box = (Rect2f) detection.corners_img;
    Point3f center = detection.center_base;

    float u = bounding_box.x, v = bounding_box.y;
    float ratio = bounding_box.width / bounding_box.height;
    float area = bounding_box.area();

    // debug("Observation: u={}, v={}, r={}, a={}", u, v, ratio, area);

    return (Mat_<float>(7, 1) << u, v, ratio, area, center.x, center.y, center.z);
}