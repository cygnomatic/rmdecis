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
    VideoWriter writer;

    bool is_recording = false;

    int frame_position = 0;

    int width, height;
    float playback_speed = 1.0f;
    float record_speed = 0.5f;
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
     * Read "pos"-th frame of the video.
     * @param pos n-th Frame.
     * @warning THIS FUNCTION WILL BROKE THE TRACK BAR! Use getFrame() instead.
     * @return
     */
    Mat getFrame(int pos);

    /**
     * Show a frame.
     * @param frame The frame.
     */
    void update(Mat &frame);

    /**
     * Set playback speed. Usually set to 0.1 for better observation.
     * @param speed Playback speed. Default 1.0f
     */
    void setPlaybackSpeed(float speed);

    /**
     * Set record speed.
     * The final record speed = original video speed * (playback speed * record speed)
     * @param speed Record speed. Default 0.5f.
     */
    void setRecordSpeed(float speed);

private:

    void closeStream();

    void printInfo() const;

    void toggleRecording();

};

#endif //CYGNOIDES_DECISION_SIMPLE_VIDEO_PLAYER_H
