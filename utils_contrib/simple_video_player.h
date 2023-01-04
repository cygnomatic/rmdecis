//
// Created by catslashbin on 23-1-4.
//

#ifndef CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
#define CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include "functional"

using namespace cv;
using namespace std;

class SimpleVideoPlayer
{
public:

    VideoCapture cap;
    int track_bar_position = 0;
    int frame_position = 0;
    float fps;

    explicit SimpleVideoPlayer(const String &path);

    ~SimpleVideoPlayer();

    Mat getFrame();

    void update(Mat &frame);

    void closeStream();

};

#endif //CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
