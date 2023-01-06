//
// Created by catslashbin on 23-1-4.
//

#ifndef CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
#define CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <functional>

using namespace cv;
using namespace std;

class SimpleVideoPlayer
{
public:

    VideoCapture cap;
    int track_bar_position = 0;
    int frame_position = 0;
    float playback_speed;
    float fps;

    /**
     * A simple video player. Featured with video pausing and process adjustment.
     * @param path Video path.
     */
    explicit SimpleVideoPlayer(const String &path);

    ~SimpleVideoPlayer();

    /**
     * Read a new frame.
     * @return New frame.
     */
    Mat getFrame();

    /**
     * Show a frame.
     * @param frame The frame.
     */
    void update(Mat &frame);

    float setPlaybackSpeed(float speed);

    void closeStream();

};

#endif //CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
