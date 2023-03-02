//
// Created by catslashbin on 23-1-13.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "rmdecis/core.h"

using cv::Mat;
using cv::rectangle;

int main() {

    spdlog::set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../data/vision_out/vision_result_.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.avi");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(0.2);

    Tracker tracker(cfg);
    TrackArmorInfo track_info;

    int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.length");

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto detect_result = vision_output.getData(player.frame_position);

        std::vector<ArmorInfo> armor_infos;
        for (auto &a: detect_result.armor_info) {
            cv::Rect2f bbox = a.corners_img.getBoundingBox();
            if (bbox.x <= 0 || bbox.x + bbox.width >= width || bbox.y <= 0 || bbox.y + bbox.height >= height)
                continue;
            armor_infos.emplace_back(a);
        }

        for (auto &t: armor_infos) {
            // Original input
            drawPolygons(frame, cv::minAreaRect(std::vector<cv::Point2f>(t.corners_img)), {255, 255, 0}, 5);
        }

        for (auto &p: tracker.getTracks(true)) {
            // Last probationary tracker
            drawPolygons(frame, p.second.last_bbox_, {255, 255, 0}, 5);
            if (!armor_infos.empty()) {
                auto similarity = fmt::format("{:.2f}", p.second.calcSimilarity(armor_infos.at(0), detect_result.time));
                cv::putText(frame, similarity, {(int) p.second.last_bbox_.center.x, (int) p.second.last_bbox_.center.y},
                            cv::FONT_HERSHEY_SIMPLEX, 1, {0, 200, 200}, 2);
            }
        }

        for (auto &p: tracker.getTracks()) {
            // Last tracker
            drawPolygons(frame, p.second.last_bbox_, {0, 255, 255}, 5);
        }

        // Update trackers
        reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);
        tracker.update(armor_infos, detect_result.time);

        // for (auto &p: tracker.getTracks()) {
        //
        //     // track_info = p.second.predict(detect_result.time + 10);
        //     // drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 50, 50}, 10);
        //     // rectangle(frame, track_info.bbox, {0, 50, 50}, 5);
        //     //
        //     // track_info = p.second.predict(detect_result.time + 5);
        //     // drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 150, 150}, 10);
        //     // rectangle(frame, track_info.bbox, {0, 150, 150}, 5);
        //
        //     track_info = p.second.predict(detect_result.time);
        //     drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 150, 150}, 10);
        //     rectangle(frame, track_info.bbox, {0, 150, 150}, 5);
        // }

        player.update(frame);
    }
}
