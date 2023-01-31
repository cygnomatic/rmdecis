//
// Created by catslashbin on 23-1-27.
//

#include "decision/basic_aiming.h"
#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"

using namespace cv;

int main() {

    set_level(spdlog::level::debug);

    // Camera & detection part output simulation
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");

    // Initialize BasicAiming with path to camera calibration coeffs file & config loader.
    ConfigLoader config_loader("../../config");
    BasicAiming basic_aiming(config_loader);

    auto compensate_t = config_loader.load("aiming").get<float>("basic.compensateTime");
    while (true) {

        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        // `vision_output.getData(player.frame_position)` simulates the detection part.
        // It returns a `DetectArmorsFrame` instance, which is the result of detection.
        DetectArmorsFrame detection = vision_output.getData(player.frame_position);

        // Call `basic_aiming.update()` to get the angle to aim.
        EulerAngles result = basic_aiming.update(detection);

        // Simply ignore the following parts. They are just for displaying the result.
        String result_display = fmt::format("Yaw: {:.2f}, Pitch: {:.2f}", result.yaw, result.pitch);
        putText(frame, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

        for (auto &t: detection.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        for (auto &p: basic_aiming.tracker.getTracks()) {
            auto track_info = p.second.predict(detection.time + compensate_t);
            auto center = basic_aiming.transformer.cam2img(track_info.center_gimbal);
            drawPoint(frame, center, {0, 255, 255}, 10);
            rectangle(frame, track_info.bbox, {0, 255, 255}, 5);
        }

        player.update(frame);
    }
}
