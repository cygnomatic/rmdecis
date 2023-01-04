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

    while (true)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;


        auto corners_img = vision_output.getData(player.frame_position).corners_img_coord;

        try
        {
            auto corners_cam = camera_calib.armorSolvePnP(getArmorModelCoord(SMALL_ARMOR), corners_img);
            info("Distance: {}", norm(corners_cam.getCenter()));
        } catch (Exception &e)
        {
            error("SolvePnP failed: {}", e.what());
        }

        drawArmorCorners(frame, corners_img, {0, 0, 255});

        player.update(frame);
    }
}