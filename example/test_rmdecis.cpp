//
// Created by catslashbin on 23-2-4.
//

#include <rmdecis/basic_aiming.h>
#include <iostream>

using namespace cv;

ArmorFrameInput getDetectionResult() {
    /* Construct DetectArmorInfo */
    auto corners = RectCorners2f({100, 200}, {100, 300}, {200, 200}, {200, 300});
    auto armor = DetectArmorInfo(STANDARD_3, corners, 1.0);

    /* Construct DetectArmorsFrame */
    int seq_idx = 0; // Frame seq index
    Time time = {43453, 122}; // Frame capture time
    std::vector<DetectArmorInfo> armor_info = {armor}; // Detected armor list

    /* Construct RobotState */
    auto robot_state = RobotState(0.f, 0.f, 15000, true);

    return {seq_idx, time, robot_state, armor_info};
}

int main() {
    /* Load config */
    Config cfg("../config/config.yml");
    BasicAiming basic_aiming(cfg);

    /* Predict */
    EulerAngles angles = basic_aiming.update(getDetectionResult());

    /* Print result (should be "nan nan") */
    std::cout << angles.pitch << " " << angles.yaw << std::endl;
}