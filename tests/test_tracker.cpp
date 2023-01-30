//
// Created by catslashbin on 23-1-13.
//

//
// Created by catslashbin on 23-1-6.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/transformer.h"

using cv::Mat;
using cv::rectangle;

int main() {

    set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    CameraCalib camera_calib("../../config/camera.yml");
    ConfigLoader config_loader("../../config");
    Transformer transformer(camera_calib);


    player.setPlaybackSpeed(1);

    Tracker tracker(config_loader);
    TrackArmorInfo track_info;

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto pred_result = vision_output.getData(player.frame_position);
        transformer.reconstructArmor3D(pred_result.armor_info);

        tracker.update(pred_result);

        for (auto &t: pred_result.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {


            track_info = p.second.predict(pred_result.time);

            auto tmp = transformer.cam2img(track_info.center_gimbal);
            drawPoint(frame, tmp, {0, 255, 255}, 10);
            debug("center: x={}, y={}", tmp.x, tmp.y);
            rectangle(frame, track_info.bbox, {0, 255, 255}, 5);

            track_info = p.second.predict(pred_result.time + 5);
            drawPoint(frame, transformer.cam2img(track_info.center_gimbal), {0, 150, 150}, 10);
            rectangle(frame, track_info.bbox, {0, 150, 150}, 5);

            track_info = p.second.predict(pred_result.time + 10);
            drawPoint(frame, transformer.cam2img(track_info.center_gimbal), {0, 50, 50}, 10);
            rectangle(frame, track_info.bbox, {0, 50, 50}, 5);

            // debug("i={}, bbox.x={}, bbox.y={}", i, track_info.x, track_info.y);
        }

        player.update(frame);
    }
}
