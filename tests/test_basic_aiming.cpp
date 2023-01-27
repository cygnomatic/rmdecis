//
// Created by catslashbin on 23-1-27.
//

#include "decision/basic_aiming.h"
#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"

using cv::Mat;
using cv::rectangle;

int main() {

    set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");

    BasicAiming basic_aiming("../../config/cam_cali_coeffs.yml");

    player.setPlaybackSpeed(1);

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto detection = vision_output.getData(player.frame_position);

        for (auto &t: detection.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        auto result = basic_aiming.update(detection);
        info("Yaw: {}, Pitch: {}", result.yaw, result.pitch);

        for (auto &p: basic_aiming.tracker.getTracks()) {

            auto track_info = p.second.predict(detection.time);

            auto tmp = basic_aiming.transformer.cam2img(track_info.center_gimbal);
            drawPoint(frame, tmp, {0, 255, 255}, 10);
            debug("center: x={}, y={}", tmp.x, tmp.y);
            rectangle(frame, track_info.bbox, {0, 255, 255}, 5);
        }

        player.update(frame);
    }
}
