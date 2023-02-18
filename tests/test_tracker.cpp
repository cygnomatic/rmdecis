//
// Created by catslashbin on 23-1-13.
//

//
// Created by catslashbin on 23-1-6.
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

    SimulateVisionOutput vision_output("../data/vision_out/vision_result_1.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input_1.avi");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(1);

    Tracker tracker(cfg);
    TrackArmorInfo track_info;

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto detect_result = vision_output.getData(player.frame_position);
        reconstructor.reconstructArmor(detect_result);

        tracker.update(detect_result);

        for (auto &t: detect_result.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {

            track_info = p.second.predict(detect_result.time + 10);
            drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 50, 50}, 10);
            rectangle(frame, track_info.bbox, {0, 50, 50}, 5);

            track_info = p.second.predict(detect_result.time + 5);
            drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 150, 150}, 10);
            rectangle(frame, track_info.bbox, {0, 150, 150}, 5);

            track_info = p.second.predict(detect_result.time);
            drawPoint(frame, reconstructor.cam2img(track_info.center_gimbal), {0, 150, 150}, 10);
            rectangle(frame, track_info.bbox, {0, 150, 150}, 5);


            // debug("i={}, bbox.x={}, bbox.y={}", i, track_info.x, track_info.y);
        }

        player.update(frame);
    }
}
