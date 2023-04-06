//
// Created by catslashbin on 22-11-21.
//

#include "armor_track.h"
#include "track_kalman.h"
#include "rmdecis/core.h"
#include "reconstructor/reconstructor.h"

using namespace cv;

ArmorTrack::ArmorTrack(int tracking_id, const ArmorInfo &detection, int frame_seq, Config &cfg)
        : track_kalman(TrackKalman(cfg, detection, frame_seq)), tracking_id(tracking_id),
          k_dilate_(cfg.get<float>("tracker.bboxDilate", 3.0f)) {
    init(detection);
}

void ArmorTrack::init(const ArmorInfo &detection) {
    id_cnt[detection.facility_id]++;

    last_bbox_ = cv::minAreaRect((std::vector<Point2f>) detection.corners_img);
    last_center_proj_ = detection.reconstructor->cam2img(detection.target_cam);
    last_image_corners_ = detection.corners_img;
}


void ArmorTrack::correct(const ArmorInfo &detection, int frame_seq) {

    // ***********************************************************************************************
    // NOTE: ALWAYS REMEMBER TO CHANGE SIMULTANEOUSLY `ArmorTrack::init` WHEN YOU CHANGE THIS FUNCTION
    // ***********************************************************************************************

    track_kalman.correct(detection, frame_seq); // pre(t-1, t) -> post(t, t)
    id_cnt[detection.facility_id]++;

    last_bbox_ = cv::minAreaRect((std::vector<Point2f>) detection.corners_img);
    last_center_proj_ = detection.reconstructor->cam2img(detection.target_cam);
    last_image_corners_ = detection.corners_img;
}

TrackArmorInfo ArmorTrack::predict(int frame_seq) {
    return track_kalman.predict(frame_seq);
}

float ArmorTrack::calcSimilarity(const ArmorInfo &detection, int frame_seq) {

    float iou = calculateIoU(last_bbox_, minAreaRect(std::vector<Point2f>(detection.corners_img)), k_dilate_);
    // float iou_pred = calculateIoU(predict(frame_seq).bbox, detection.corners_img.getBoundingBox(), k_dilate_);
    float id_similarity = calcIdSimilarity(detection.facility_id);
    // float center_dist_similarity = calcCenterDistSimilarity(detection.target_world);

    float ret = iou * 0.75f + id_similarity * 0.25f; // + center_dist_similarity * 0.0f;
    assert(!isnanf(ret));

    return ret;
}

float ArmorTrack::calcIdSimilarity(FacilityID id) {
    int sum = std::accumulate(id_cnt.begin(), id_cnt.end(), 2);
    return (float) (id_cnt[id] + 1) /
           (float) sum; // Here we artificially add one positive and one negative. To smoothen the result.
}

float ArmorTrack::calcCenterDistSimilarity(const Point3f &center) {
    Point3f center_pred = {track_kalman.kf.statePost.at<float>(STATE_X),
                           track_kalman.kf.statePost.at<float>(STATE_Y),
                           track_kalman.kf.statePost.at<float>(STATE_Z)};
    float dist = float(norm((center_pred - center)));
    return max((200.f - dist) * 0.005f, 0.f);
}