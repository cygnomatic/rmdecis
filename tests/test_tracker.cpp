//
// Created by catslashbin on 23-1-13.
//

//
// Created by catslashbin on 23-1-6.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/tracker/tracker.h"

using cv::Mat;
using cv::rectangle;

int main() {

    set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    player.setPlaybackSpeed(0.1);

    Tracker tracker;

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto pred_result = vision_output.getData(player.frame_position);
        tracker.update(pred_result);

        for (auto &t: pred_result.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 0, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {
            auto bbox = p.second.predict(0);
            rectangle(frame, bbox, {0, 255, 255}, 2);
        }

        player.update(frame);
    }
}
