//
// Created by catslashbin on 23-1-4.
//

#include "../utils_contrib/simple_video_player.h"

void bgr2gray(Mat & img)
{
    cvtColor(img, img, CV_BGR2GRAY);
}

int main()
{
    SimpleVideoPlayer player;
    player.setImgProcessFunc(bgr2gray);
    player.play("../data/video_test3.avi");
}