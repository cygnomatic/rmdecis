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

    player.setPlaybackSpeed(1);

    Tracker tracker(cfg);
    TrackArmorInfo track_info;

    int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.length");

    while (true) {
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

        reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);
        for (auto &a: armor_infos) {
            // ::info("x: {}, y: {}, z: {}", a.target_world.x, a.target_world.y, a.target_world.z);
        }
        tracker.update(armor_infos, detect_result.seq_idx);

        for (auto &t: detect_result.armor_info) {
            drawArmorCorners(frame, t.corners_img, {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {

            // TODO: world to cam
            track_info = p.second.predict(detect_result.seq_idx);
            drawPoint(frame, reconstructor.cam2img(track_info.target_world), {0, 250, 250}, 10);

            track_info = p.second.predict(detect_result.seq_idx + 1);
            drawPoint(frame, reconstructor.cam2img(track_info.target_world), {0, 150, 150}, 10);

            track_info = p.second.predict(detect_result.seq_idx + 2);
            drawPoint(frame, reconstructor.cam2img(track_info.target_world), {0, 100, 100}, 10);

        }

        player.update(frame);
    }
}
