//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../utils/cv_utils.h"

int main()
{
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");

    while (true)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        for (auto pred_result: vision_output.getData(player.frame_position))
        {
            drawArmorCorners(frame, pred_result.corners_img_coord, {0, 0, 255});
        }

        player.update(frame);
    }
}