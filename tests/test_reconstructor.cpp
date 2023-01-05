//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../utils/cv_utils.h"
#include "../reconstructor/camera_calib.h"
#include "../typing/armor.h"

int main()
{
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    CameraCalib camera_calib("../../config/cam_cali_coeffs.yml");

    Mat rvec, tvec;

    while (true)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;


        for (auto pred_result: vision_output.getData(player.frame_position))
        {
            drawArmorCorners(frame, pred_result.corners_img_coord, {0, 0, 255});
            camera_calib.armorSolvePnP(getArmorModelCoord(SMALL_ARMOR), pred_result.corners_img_coord, rvec, tvec);
            info("Distance: {}", norm(tvec));
        }

        player.update(frame);
    }
}