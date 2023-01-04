//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils/cv_utils.h"


int main()
{
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    while (true)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;
        player.update(frame);
    }
}