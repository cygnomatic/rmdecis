//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/reconstructor/reconstructor.h"

#include <fmt/format.h>

using namespace cv;

int main() {
    SimulateVisionOutput vision_output("../data/vision_out/vision_result_1.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input_1.avi");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(0.5);

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto frame_inp = vision_output.getData(player.frame_position);
        reconstructor.reconstructArmor(frame_inp);

        for (auto &armor: frame_inp.armor_info) {
            // String result_display = fmt::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", armor.target_world.x,
            //                                     armor.target_world.y, armor.target_world.z);
            // putText(frame, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

            drawArmorCorners(frame, armor.corners_img, {255, 255, 255});

            Point3f arrow_top_cam = Transformer::modelToCam({Point3f(0, 0, 100)}, armor.trans_model2cam);
            Point3f arrow_bot_cam = armor.target_cam;

            if ((arrow_top_cam - arrow_bot_cam).z > 0) {
                arrow_top_cam = 2 * arrow_bot_cam - arrow_top_cam;
            }

            auto arrow = reconstructor.cam_calib.projectToImage({arrow_bot_cam, arrow_top_cam});
            arrowedLine(frame, arrow.at(0), arrow.at(1), {255, 0, 255}, 3);

            // break;
        }

        player.update(frame);
    }
}