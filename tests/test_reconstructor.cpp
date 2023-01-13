//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/reconstructor/camera_calib.h"
#include "../src/utils/cv_utils.h"

#include <fmt/format.h>

int main() {
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    CameraCalib camera_calib("../../config/cam_cali_coeffs.yml");

    player.setPlaybackSpeed(0.5);

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;


        for (auto pred_result: vision_output.getData(player.frame_position).armor_info) {
            drawArmorCorners(frame, pred_result.corners_img, {0, 0, 255});
            auto trans_model2cam = camera_calib.armorSolvePnP(ArmorCorners3d(SMALL_ARMOR), pred_result.corners_img);

            // String dist = "Distance: " + std::to_string(norm(trans_model2cam.tvec) / 1000);
            // putText(frame, dist, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

            String rot = fmt::format("tvec [x: {:.2f}, y: {:.2f}, z: {:.2f}]", trans_model2cam.tvec.at<double>(0),
                                     trans_model2cam.tvec.at<double>(1), trans_model2cam.tvec.at<double>(2));
            putText(frame, rot, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

            // info("Rvec [x: {:.2f}, y: {:.2f}, z: {:.2f}]", trans_model2cam.rvec.at<double>(0), trans_model2cam.rvec.at<double>(1), trans_model2cam.rvec.at<double>(2));

            auto reprojected_corners = (ArmorCorners2d) camera_calib.projectToImage(
                    std::vector<Point3f>(ArmorCorners3d(SMALL_ARMOR)), trans_model2cam);
            drawArmorCorners(frame, reprojected_corners, {255, 255, 0});

            camera_calib.drawAxes(frame, trans_model2cam);
            break;
        }

        player.update(frame);
    }
}