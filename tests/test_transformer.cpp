//
// Created by catslashbin on 23-3-7.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/reconstructor/reconstructor.h"

#include <fmt/format.h>

using namespace cv;

int main() {
    SimulateVisionOutput vision_output("../data/vision_out/vision_result_.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.avi");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(0.5);

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto frame_inp = vision_output.getData(player.frame_position);
        std::vector<ArmorInfo> armor_infos;
        for (auto &a: frame_inp.armor_info) {
            armor_infos.emplace_back(a);
        }
        reconstructor.reconstructArmors(armor_infos, frame_inp.robot_state);

        Transformer &transformer = reconstructor.transformer;
        CameraCalib &camera_calib = reconstructor.cam_calib;

        auto center = Point3f(10, 0, 0);
        auto p_c = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 0) + center));
        auto p_x = camera_calib.projectToImage(transformer.worldToCam(Point3f(1, 0, 0) + center));
        auto p_y = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 1, 0) + center));
        auto p_z = camera_calib.projectToImage(transformer.worldToCam(Point3f(0, 0, 1) + center));

        line(frame, p_c, p_x, {255, 0, 0}, 3);
        line(frame, p_c, p_y, {0, 255, 0}, 3);
        line(frame, p_c, p_z, {0, 0, 255}, 3);

        for (auto &armor: armor_infos) {
            // String result_display = fmt::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", armor.target_world.x,
            //                                     armor.target_world.y, armor.target_world.z);
            // putText(frame, result_display, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

            drawArmorCorners(frame, armor.corners_img, {255, 255, 255});

            auto p_world = camera_calib.projectToImage(transformer.worldToCam(armor.target_world));
            drawPoint(frame, p_world, {255, 255, 0}, 10);

            auto p_gimbal = camera_calib.projectToImage(transformer.gimbalToCam(armor.target_gimbal));
            drawPoint(frame, p_gimbal, {255, 0, 255}, 10);

            auto p_cam = camera_calib.projectToImage(armor.target_cam);
            drawPoint(frame, p_cam, {0, 255, 255}, 10);


            // Point3f arrow_top_cam = Transformer::modelToCam({Point3f(0, 0, 100)}, armor.trans_model2cam);
            // Point3f arrow_bot_cam = armor.target_cam;
            //
            // if ((arrow_top_cam - arrow_bot_cam).z > 0) {
            //     arrow_top_cam = 2 * arrow_bot_cam - arrow_top_cam;
            // }
            //
            // auto arrow = reconstructor.cam_calib.projectToImage({arrow_bot_cam, arrow_top_cam});
            // arrowedLine(frame, arrow.at(0), arrow.at(1), {255, 0, 255}, 3);
            // break;
        }

        player.update(frame);
    }
}