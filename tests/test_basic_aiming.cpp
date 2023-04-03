//
// Created by catslashbin on 23-1-27.
//

#include "rmdecis_impl/basic_aiming_impl.h"
#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include <string>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main() {

    spdlog::set_level(spdlog::level::debug);

    // Camera & detection part output simulation
    SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.mp4");
    player.setRecordSpeed(2);

    // Initialize BasicAimingImpl with path to camera calibration coeffs file & config loader.
    Config cfg("../config/config.yml");
    BasicAiming basic_aiming(cfg);

    // For debug
    std::string debug_msg;

    while (true) {

        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        // `vision_output.getData(player.frame_position)` simulates the detection part.
        // It returns a `DetectArmorsFrame` instance, which is the result of detection.
        ArmorFrameInput detection = vision_output.getData(player.frame_position);

        // Call `basic_aiming.update()` to get the angle to aim.
        EulerAngles result = basic_aiming.update(detection, &frame, &debug_msg);

        player.update(frame);
        info(debug_msg);
    }
}
