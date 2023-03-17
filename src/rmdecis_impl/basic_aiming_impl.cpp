//
// Created by catslashbin on 23-1-24.
//

#include "basic_aiming_impl.h"
#include "reconstructor/transformer.h"
#include "rmdecis/core.h"
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <spdlog/fmt/bundled/core.h>

using namespace cv;

EulerAngles BasicAiming::BasicAimingImpl::update(ArmorFrameInput detection, cv::Mat *debug_img) {

    /* DEBUG */
    enable_debug = (debug_img != nullptr);
    if (enable_debug)
        assert(!debug_img->empty());
    /* !DEBUG */

    std::vector<ArmorInfo> armor_infos;
    for (const auto &armor: detection.armor_info) {
        cv::Rect2i bbox = armor.corners_img.getBoundingBox();
        if (bbox.x <= 0 || bbox.x + bbox.width >= frame_width_
            || bbox.y <= 0 || bbox.y + bbox.height >= frame_height_)
            continue;
        if (armor.detection_confidence < 0.5)
            continue;
        armor_infos.emplace_back(armor);
    }

    // Reconstruct armors
    reconstructor.reconstructArmors(armor_infos, detection.robot_state);

    /* DEBUG */
    if (enable_debug && enable_show_vision_input) {
        // Draw detection input
        for (auto &t: armor_infos) {
            drawArmorCorners(*debug_img, t.corners_img, {255, 255, 0}, 5);
            cv::putText(*debug_img, fmt::format("{:.2f}", t.detection_confidence),
                            (t.corners_img[0] + t.corners_img[2]) / 2, cv::FONT_HERSHEY_SIMPLEX, 1, {255, 255, 0}, 2);
        }
    }

    if (enable_debug && enable_show_tracker) {

        // Robot state input
        cv::putText(*debug_img, fmt::format("yaw: {:.2f}, pitch: {:.2f}", detection.robot_state.gimbal_yaw, detection.robot_state.gimbal_pitch), 
                            Point(100, 200), cv::FONT_HERSHEY_SIMPLEX, 1, {100, 255, 255}, 2);

        // Reconstructor result
        if (!armor_infos.empty()) {
            auto t = armor_infos[0];
            cv::putText(*debug_img, fmt::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", t.target_world.x, t.target_world.y, t.target_world.z), 
                            Point(100, 300), cv::FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 2);
        }


        Transformer &transformer = reconstructor.transformer;
        CameraCalib &camera_calib = reconstructor.cam_calib;

        // auto cam_center_in_world = transformer.camToWorld(Point3f(10000, 0, 0));
        // cv::putText(*debug_img, fmt::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", cam_center_in_world.x, cam_center_in_world.y, cam_center_in_world.z), 
        //                     Point(100, 300), cv::FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 2);

        auto center = Point3f(10000, 0, 0);
        auto p_c = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 0) + center));
        auto p_x = camera_calib.projectToImage(transformer.worldToCam(Point3f(1000, 0, 0) + center));
        auto p_y = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 1000, 0) + center));
        auto p_z = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 1000) + center));

        line(*debug_img, p_c, p_x, {255, 0, 0}, 3);
        line(*debug_img, p_c, p_y, {0, 255, 0}, 3);
        line(*debug_img, p_c, p_z, {0, 0, 255}, 3);

        // Original probationary tracker
        for (auto &p: tracker.getTracks(true)) {
            if (!armor_infos.empty()) {

                auto &t = p.second;
                ArmorInfo &a = armor_infos.at(0);

                auto pred_center_proj = a.reconstructor->cam2img(
                        a.reconstructor->transformer.worldToCam(t.predict(detection.seq_idx).target_world));
                auto pred_bbox = cv::RotatedRect(((t.last_bbox_.center - t.last_center_proj_) + pred_center_proj),
                                                 t.last_bbox_.size, t.last_bbox_.angle);

                drawPolygons(*debug_img, pred_bbox, {150, 150, 150}, 2);

                auto similarity = fmt::format("{:.2f}", p.second.calcSimilarity(armor_infos.at(0), detection.seq_idx));
                cv::putText(*debug_img, similarity,
                            {(int) p.second.last_bbox_.center.x, (int) p.second.last_bbox_.center.y},
                            cv::FONT_HERSHEY_SIMPLEX, 1, {150, 150, 150}, 2);
            }
        }
    }
    /* !DEBUG */

    tracker.update(armor_infos, detection.seq_idx);

    /* DEBUG */
    if (enable_debug && enable_show_tracker) {
        // Draw tracker predictions
        for (auto &p: tracker.getTracks()) {
            TrackArmorInfo track_info = p.second.predict(detection.seq_idx);
            drawPoint(*debug_img,
                      reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 250, 250}, 10);

            track_info = p.second.predict(detection.seq_idx + 5);
            drawPoint(*debug_img,
                      reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 150, 150}, 10);

            track_info = p.second.predict(detection.seq_idx + 10);
            drawPoint(*debug_img,
                      reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 100, 100}, 10);
        }

        // Draw trackers
        for (auto &p: tracker.getTracks()) {
            drawPolygons(*debug_img, p.second.last_bbox_, {0, 255, 255}, 5);
        }
    }
    /* !DEBUG */

    auto tracks_map = tracker.getTracks();
    if (tracks_map.find(last_aiming_id_) == tracks_map.end()) {
        // Last track lost, update target track
        last_aiming_id_ = chooseNextTarget(tracks_map, detection.seq_idx);
    }

    // Check if there is a target
    if (last_aiming_id_ != -1) {
        auto pred_angle = predictFromTrack(tracks_map.at(last_aiming_id_),
                                           detection.seq_idx + compensate_frame);

        // Avoid nan from predictFromTrack
        if (!(std::isnan(pred_angle.yaw) || std::isnan(pred_angle.pitch))) {
            last_aiming_angle_ = pred_angle;
        } else {
            warn("Got nan from predictFromTrack!");
        }
    }

    /* DEBUG */
    // if (enable_debug) {
    //     String result_display = fmt::format("Yaw: {:.2f}, Pitch: {:.2f}", last_aiming_angle_.yaw,
    //                                         last_aiming_angle_.pitch);
    //     putText(*debug_img, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);
    // }
    /* !DEBUG */

    return last_aiming_angle_;

}

/**
 * WARNING: The return of this func can be nan!
 */
EulerAngles BasicAiming::BasicAimingImpl::predictFromTrack(ArmorTrack &track, int frame_seq) {

    float horizontal_dist, vertical_dist, yaw, pitch;

    TrackArmorInfo target_info = track.predict(frame_seq);
    Point3f center = target_info.target_world;
    reconstructor.solveAngle(center, &yaw, &horizontal_dist, &vertical_dist);

    // TODO: calcShootAngle can return pitch with nan if there is no solution.
    pitch = compensator.calcShootAngle(ballet_init_speed, horizontal_dist, vertical_dist);

    // FIXME: Return pitch should multiply -1
    return EulerAngles(yaw, pitch);

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
        float dist = sqrt(powf(pred_angle.yaw - last_aiming_angle_.yaw, 2)
                          * powf(pred_angle.pitch - last_aiming_angle_.pitch, 2));

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
        : reconstructor(cfg), tracker(cfg),
          compensator(cfg.get<float>("aiming.basic.airResistanceConst", 0.1)),
          frame_width_(cfg.get<int>("camera.width")),
          frame_height_(cfg.get<int>("camera.height")),
          enable_show_vision_input(cfg.get<bool>("aiming.basic.debugShowVisionInput", true)),
          enable_show_tracker(cfg.get<bool>("aiming.basic.debugShowTracker", true)) {

    // TODO: Time to compensate frame
    compensate_frame = cfg.get<int>("aiming.basic.compensateTime", 0);
}
