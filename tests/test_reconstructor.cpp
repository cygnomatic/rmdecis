//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/reconstructor/camera_calib.h"
#include "../src/utils/cv_utils.h"

int main()
{
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    CameraCalib camera_calib("../../config/cam_cali_coeffs.yml");

    while (true)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;


        for (auto pred_result: vision_output.getData(player.frame_position))
        {
            drawArmorCorners(frame, pred_result.corners_img_coord, {0, 0, 255});
            auto trans = camera_calib.armorSolvePnP(ArmorCorners3d(SMALL_ARMOR), pred_result.corners_img_coord);

            String dist = "Distance: " + std::to_string(norm(trans.tvec) / 1000);
            putText(frame, dist, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);

            camera_calib.drawAxes(frame, trans);
        }

        player.update(frame);
    }
}