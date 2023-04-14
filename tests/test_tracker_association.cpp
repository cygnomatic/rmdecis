//
// Created by catslashbin on 23-1-13.
//

// using cv::Mat;
// using cv::rectangle;
//
// int main() {
//
//     spdlog::set_level(spdlog::level::debug);
//
//     SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
//     SimpleVideoPlayer player("../data/vision_out/video_input.mp4");
//
//     Config cfg("../config/config.yml");
//     Reconstructor reconstructor(cfg);
//
//     player.setPlaybackSpeed(0.2);
//     Tracker tracker(cfg);
//
//     int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.height");
//
//     for (int i = 0;; i++) {
//
//         Mat frame = player.getFrame();
//         if (frame.empty())
//             break;
//
//         auto detect_result = vision_output.getData(player.frame_position);
//
//         std::vector<ArmorInfo> armor_infos;
//         for (auto &a: detect_result.armor_info) {
//             cv::Rect2f bbox = a.corners_img.getBoundingBox();
//             if (bbox.x <= 0 || bbox.x + bbox.width >= width || bbox.y <= 0 || bbox.y + bbox.height >= height)
//                 continue;
//             if (a.detection_confidence < 0.55f)
//                 continue;
//             armor_infos.emplace_back(a);
//         }
//         reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);
//
//         // for (auto &p: tracker.getTracks(true)) {
//         //     // Last probationary tracker
//         //     drawPolygons(frame, p.second.last_bbox_, {0, 255, 255}, 5);
//         //
//         //     for (auto &a : armor_infos) {
//         //         auto &t = p.second;
//         //
//         //         auto pred_bbox = minAreaRect(std::vector<cv::Point2f>(a.corners_img));
//         //         drawPolygons(frame, pred_bbox, {255, 255, 0}, 5);
//         //
//         //         auto similarity = fmt::format("{:.2f}",
//         //                                       p.second.calcSimilarity(armor_infos.at(0), detect_result.seq_idx));
//         //
//         //         cv::putText(frame, similarity, {(int) p.second.last_bbox_.center.x, (int) p.second.last_bbox_.center.y},
//         //                     cv::FONT_HERSHEY_SIMPLEX, 1, {0, 200, 200}, 2);
//         //     }
//         // }
//
//         int cnt = 0;
//         for (auto &t: tracker.getTracks(true)) {
//             float max_sim = 0.0;
//             for (auto &a: armor_infos) {
//                 max_sim = std::max(t.second.calcSimilarity(a, detect_result.seq_idx), max_sim);
//             }
//
//             // Draw last (prob) tracks
//             drawPolygons(frame, t.second.last_bbox_, {200, 0, 200});
//             cv::putText(frame, fmt::format("{:.2f}", max_sim),
//                         {(int) t.second.last_bbox_.center.x, (int) t.second.last_bbox_.center.y},
//                         cv::FONT_HERSHEY_SIMPLEX, 1, {200, 0, 200});
//
//             if (cnt > 5) break;
//             else ++cnt;
//         }
//
//         // Draw detection result
//         for (auto &a : armor_infos) {
//             drawPolygons(frame, cv::minAreaRect(std::vector<cv::Point2f>(a.corners_img)), {200, 200, 200});
//         }
//
//         // Update trackers
//         tracker.update(armor_infos, detect_result.seq_idx);
//
//         // Draw new tracker
//         for (auto &t: tracker.getTracks()) {
//             drawPolygons(frame, t.second.last_bbox_, {0, 200, 200});
//         }
//
//         player.update(frame);
//     }
// }

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "rmdecis/core.h"
#include "utils/preprocess.h"

using cv::Mat;
using cv::rectangle;

int main() {

    spdlog::set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.mp4");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(1);
    player.setRecordSpeed(2);

    Tracker tracker(cfg);
    TrackArmorInfo track_info;

    int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.height");

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto detect_result = vision_output.getData(player.frame_position);

        std::vector<ArmorInfo> armor_infos = detectionToInfo(detect_result.armor_info, width, height,
                                                             0.6, CompetitionRule(cfg));

        reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);

        int cnt = 0;
        for (auto &t: tracker.getTracks(true)) {
            float max_sim = 0.0;
            for (auto &a: armor_infos) {
                max_sim = std::max(t.second.calcSimilarity(a, detect_result.seq_idx), max_sim);
            }

            // Draw last (prob) tracks
            drawPolygons(frame, t.second.predReprojRect(&reconstructor, detect_result.seq_idx), {200, 0, 200});
            cv::putText(frame, fmt::format("{:.2f}", max_sim),
                        {(int) t.second.last_bbox_.center.x, (int) t.second.last_bbox_.center.y},
                        cv::FONT_HERSHEY_SIMPLEX, 1, {200, 0, 200});

            if (cnt > 5) break;
            else ++cnt;
        }

        tracker.update(armor_infos, detect_result.seq_idx);

        for (auto &t: detect_result.armor_info) {
            drawArmorCorners(frame, t.corners_img, {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {

            track_info = p.second.predict(detect_result.seq_idx + 10);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 100, 100}, 3);

            track_info = p.second.predict(detect_result.seq_idx + 5);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 150, 150}, 3);

            track_info = p.second.predict(detect_result.seq_idx);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 250, 250}, 3);
        }

        player.update(frame);
    }
}

