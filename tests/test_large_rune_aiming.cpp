//
// Created by catslashbin on 23-1-27.
//

#include "rmdecis_impl/basic_aiming_impl.h"
#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"

using namespace cv;

int main() {

    spdlog::set_level(spdlog::level::debug);

    // Camera & detection part output simulation
    SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.avi");

    // Initialize BasicAimingImpl with path to camera calibration coeffs file & config loader.
    Config cfg("../config/config.yml");
    BasicAiming basic_aiming(cfg);

    // auto compensate_t = cfg.get<float>("aiming.basic.compensateTime");
    while (true) {

        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        // `vision_output.getData(player.frame_position)` simulates the detection part.
        // It returns a `DetectArmorsFrame` instance, which is the result of detection.
        FrameInput detection = vision_output.getData(player.frame_position);

        // Call `basic_aiming.update()` to get the angle to aim.
        EulerAngles result = basic_aiming.update(detection);

        // Simply ignore the following parts. They are just for displaying the result.
        String result_display = fmt::format("Yaw: {:.2f}, Pitch: {:.2f}", result.yaw, result.pitch);
        putText(frame, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

        for (auto &t: detection.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        player.update(frame);
    }
}
