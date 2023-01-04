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
    /**
     * Set image process function.
     * @param process_img_func function called to process the image while playing.
     */
    void setImgProcessFunc(const std::function<void(Mat &)> &process_img_func);

    /**
     * Play video
     * @param path video path.
     */
    void play(const String &path);

private:

    static void defaultImgProcessFunc(Mat &img);

    function<void(Mat &)> processImgFunc = defaultImgProcessFunc;
};

#endif //CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
