//
// Created by catslashbin on 23-1-24.
//

#include "basic_aiming_impl.h"
#include "reconstructor/transformer.h"
#include "rmdecis/core.h"
#include "utils/preprocess.h"
#include "typing.h"

#define DEBUG_DETECTION if (debug_img_ != nullptr && !debug_img_->empty() && enable_show_vision_input)
#define DEBUG_DECISION if (debug_img_ != nullptr && !debug_img_->empty() && enable_show_tracker)
#define DEBUG_MSG if (debug_msg_ != nullptr)

using namespace cv;

EulerAngles BasicAiming::BasicAimingImpl::update(ArmorFrameInput detection,
                                                 cv::Mat *debug_img, std::string *debug_msg) {

    debug_img_ = debug_img;
    debug_msg_ = debug_msg;
    *debug_msg = "{}"; // Clear original msg

    // Pre-process data
    std::vector<ArmorInfo> armor_infos = detectionToInfo(detection.armor_info,
                                                         k_frame_width_, k_frame_height_, k_confidence_threshold_,
                                                         competition_rule);
    curr_pitch_ = detection.robot_state.gimbal_pitch;

    // Reconstruct armors
    reconstructor.reconstructArmors(armor_infos, detection.robot_state);

    DEBUG_DETECTION {
        // Draw detection input
        for (auto &t: armor_infos) {
            drawArmorCorners(*debug_img_, t.corners_img, {255, 255, 0}, 2);
            cv::putText(*debug_img_, fmt::format("{:.2f}", t.detection_confidence),
                        (t.corners_img[0] + t.corners_img[2]) / 2, cv::FONT_HERSHEY_SIMPLEX, 1, {200, 100, 0}, 2);
            cv::putText(*debug_img_, fmt::format("{}", t.facility_id),
                        (t.corners_img[0] + t.corners_img[2]) / 2 - Point2f(10, 10),
                        cv::FONT_HERSHEY_DUPLEX, 3, {0, 255, 255}, 3);
        }
    }

    DEBUG_DECISION {

        // Robot state input
        cv::putText(*debug_img_, fmt::format("yaw: {:.2f}, pitch: {:.2f}", detection.robot_state.gimbal_yaw,
                                             detection.robot_state.gimbal_pitch),
                    Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 1, {100, 255, 255}, 1);

        Transformer &transformer = reconstructor.transformer;
        CameraCalib &camera_calib = reconstructor.cam_calib;

        auto center = Point3f(10000, 0, 0);
        auto p_c = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 0) + center));
        auto p_x = camera_calib.projectToImage(transformer.worldToCam(Point3f(1000, 0, 0) + center));
        auto p_y = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 1000, 0) + center));
        auto p_z = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 1000) + center));

        // Avoid drawing bad projected point, or it will cause program freeze.
        if (p_c.x > -100 && p_c.x < 740 && p_c.y > -100 && p_c.y < 580) {
            line(*debug_img_, p_c, p_x, {255, 0, 0}, 2);
            line(*debug_img_, p_c, p_y, {0, 255, 0}, 2);
            line(*debug_img_, p_c, p_z, {0, 0, 255}, 2);
        }
    }

    tracker.update(armor_infos, detection.seq_idx);

    DEBUG_DECISION {
        // Draw tracker predictions
        for (auto &p: tracker.getTracks()) {
            TrackArmorInfo track_info = p.second.predict(detection.seq_idx);
            drawPoint(*debug_img_,
                      reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 250, 250}, 2);
        }

        // Draw trackers
        for (auto &p: tracker.getTracks()) {
            drawPolygons(*debug_img_, p.second.last_bbox_, {0, 255, 255}, 2);
        }

    }

    auto tracks_map = tracker.getTracks();
    if (tracks_map.find(target_id_) == tracks_map.end() ||
        (!last_enable_auto_aim && detection.robot_state.enable_auto_aim)) {
        // Choose next target if last target is lost or the operator re-enable auto aim
        target_id_ = -1; // Set target_id_ to -1 first, so the debug str in chooseNextTarget will not set.
        target_id_ = chooseNextTarget(tracks_map, detection.seq_idx);
    }
    last_enable_auto_aim = detection.robot_state.enable_auto_aim;

    DEBUG_DECISION {
        // Reconstructor result & target result
        if (target_id_ != -1) {
            auto target_trk = tracker.getTracks().at(target_id_);
            auto target_ctr_cam = target_trk.predict(detection.seq_idx).target_world;
            cv::putText(*debug_img_, fmt::format("x: {:.2f}, y: {:.2f}, z: {:.2f}",
                                                 target_ctr_cam.x, target_ctr_cam.y, target_ctr_cam.z),
                        Point(50, 150), cv::FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 1);
            drawArmorCorners(*debug_img_, target_trk.last_image_corners_, {255, 0, 0}, 5);
        }
    }

    // If there is no available target
    if (target_id_ == -1)
        return EulerAngles(NAN, NAN);

    auto pred_delta_angle = predictFromTrack(tracks_map.at(target_id_), detection.seq_idx);

    // Avoid nan from predictFromTrack
    if (std::isnan(pred_delta_angle.yaw) || std::isnan(pred_delta_angle.pitch)) {
        warn("Got nan from predictFromTrack!");
        return EulerAngles(NAN, NAN);
    }

    DEBUG_DECISION {
        String result_display = fmt::format("Yaw: {:.2f}, Pitch: {:.2f}",
                                            pred_delta_angle.yaw, pred_delta_angle.pitch);
        putText(*debug_img_, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 2);
    }

    return pred_delta_angle;
}

/**
 * WARNING: The return of this func can be nan!
 */
EulerAngles BasicAiming::BasicAimingImpl::predictFromTrack(ArmorTrack &track, int frame_seq) {

    float horizontal_dist, vertical_dist, d_yaw, d_pitch, trig_pitch, pitch;

    // Predict target without compensate time
    TrackArmorInfo target_info = track.predict(frame_seq);
    reconstructor.solveAngle(target_info.target_world, &horizontal_dist, &vertical_dist, &d_yaw, &trig_pitch);

    // Calculate compensate time
    float compensate_time = float(compensator.calcCompensateTime(horizontal_dist, vertical_dist,
                                                                 ballet_init_speed, curr_pitch_));
    compensate_time = std::isnan(compensate_time) ? 0.0f : compensate_time;

    // Predict target with compensate time
    TrackArmorInfo pred_target_info = track.predict(frame_seq, compensate_time);
    reconstructor.solveAngle(pred_target_info.target_world, &horizontal_dist, &vertical_dist, &d_yaw, &trig_pitch);

    // Calculate target pitch
    pitch = float(compensator.calcShootAngle(horizontal_dist, vertical_dist, ballet_init_speed, curr_pitch_));
    d_pitch = pitch - curr_pitch_;

    if (target_id_ != -1) {

        info("compensate_time {}, horizontal_dist {}, vertical_dist {}, currPitch {}, trigPitch {}, shootAngle {}",
             compensate_time, horizontal_dist, vertical_dist, curr_pitch_, trig_pitch, pitch);

        DEBUG_DECISION {
            drawPoint(*debug_img_,
                      reconstructor.cam2img(reconstructor.transformer.worldToCam(pred_target_info.target_world)),
                      {255, 100, 255}, 2);
        };

        DEBUG_MSG {
            *debug_msg_ = fmt::format(R"({{"horizontal_dist": {}, "vertical_dist": {}, "curr_pitch": {}}})",
                                      horizontal_dist, vertical_dist, curr_pitch_);
        }
    }

    return EulerAngles(d_yaw, d_pitch);

}

int BasicAiming::BasicAimingImpl::chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, int frame_seq) {

    if (tracks_map.empty()) {
        // No targets found.
        return -1;
    }

    float min_dist = std::numeric_limits<float>::infinity();
    int min_id = -1;

    for (auto &track_pair: tracks_map) {
        EulerAngles pred_angle = predictFromTrack(track_pair.second, frame_seq);

        // Catch nan from predictFromTrack
        if (std::isnan(pred_angle.yaw) || std::isnan(pred_angle.pitch)) {
            warn("Get nan from predictFromTrack!");
        }

        // TODO: Find a better way to select next target than calculating "distance" between two angles.
        float dist = sqrt(powf(pred_angle.yaw, 2) + powf(pred_angle.pitch, 2));

        if (dist < min_dist) {
            min_dist = dist;
            min_id = track_pair.second.tracking_id;
        }
    }

    if (min_id != -1)
        debug("New target. Track ID: {}", min_id);

    return min_id;

}

BasicAiming::BasicAimingImpl::BasicAimingImpl(Config &cfg)
        : reconstructor(cfg), tracker(cfg), compensator(cfg), competition_rule(cfg),
          k_frame_width_(cfg.get<int>("camera.width")),
          k_frame_height_(cfg.get<int>("camera.height")),
          k_confidence_threshold_(cfg.get<float>("aiming.basic.detConfidenceThreshold", 0.55)),
          enable_show_vision_input(cfg.get<bool>("aiming.basic.debugShowVisionInput", true)),
          enable_show_tracker(cfg.get<bool>("aiming.basic.debugShowTracker", true)) {}
