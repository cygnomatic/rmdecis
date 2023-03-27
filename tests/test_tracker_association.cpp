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

    SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.mp4");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(0.2);
    Tracker tracker(cfg);

    int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.height");

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
            if (a.detection_confidence < 0.55f)
                continue;
            armor_infos.emplace_back(a);
        }
        reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);

        // for (auto &p: tracker.getTracks(true)) {
        //     // Last probationary tracker
        //     drawPolygons(frame, p.second.last_bbox_, {0, 255, 255}, 5);
        //
        //     for (auto &a : armor_infos) {
        //         auto &t = p.second;
        //
        //         auto pred_bbox = minAreaRect(std::vector<cv::Point2f>(a.corners_img));
        //         drawPolygons(frame, pred_bbox, {255, 255, 0}, 5);
        //
        //         auto similarity = fmt::format("{:.2f}",
        //                                       p.second.calcSimilarity(armor_infos.at(0), detect_result.seq_idx));
        //
        //         cv::putText(frame, similarity, {(int) p.second.last_bbox_.center.x, (int) p.second.last_bbox_.center.y},
        //                     cv::FONT_HERSHEY_SIMPLEX, 1, {0, 200, 200}, 2);
        //     }
        // }

        int cnt = 0;
        for (auto &t: tracker.getTracks(true)) {
            float max_sim = 0.0;
            for (auto &a: armor_infos) {
                max_sim = std::max(t.second.calcSimilarity(a, detect_result.seq_idx), max_sim);
            }

            // Draw last (prob) tracks
            drawPolygons(frame, t.second.last_bbox_, {200, 0, 200});
            cv::putText(frame, fmt::format("{:.2f}", max_sim),
                        {(int) t.second.last_bbox_.center.x, (int) t.second.last_bbox_.center.y},
                        cv::FONT_HERSHEY_SIMPLEX, 1, {200, 0, 200});

            if (cnt > 5) break;
            else ++cnt;
        }

        // Draw detection result
        for (auto &a : armor_infos) {
            drawPolygons(frame, cv::minAreaRect(std::vector<cv::Point2f>(a.corners_img)), {200, 200, 200});
        }

        // Update trackers
        tracker.update(armor_infos, detect_result.seq_idx);

        // Draw new tracker
        for (auto &t: tracker.getTracks()) {
            drawPolygons(frame, t.second.last_bbox_, {0, 200, 200});
        }

        player.update(frame);
    }
}
