#include "simple_video_player.h"

void onTrackbarSlide(int position, void *userdata)
{
    VideoCapture *cap = (VideoCapture *) userdata;
    int frame_position = (int) (position * cap->get(CAP_PROP_FRAME_COUNT) / 100.0);
    cap->set(CAP_PROP_POS_FRAMES, frame_position);
}

void SimpleVideoPlayer::play(const String &path)
{

    VideoCapture cap(path);
    if (!cap.isOpened())
    {
        cerr << "ERROR: Unable to open the video file" << endl;
        return;
    }

    double rate = cap.get(CAP_PROP_FPS);
    cout << "Frame rate: " << rate << " fps" << endl;

    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "Frame size: " << width << " x " << height << endl;

    namedWindow("Video", WINDOW_NORMAL);
    resizeWindow("Video", width / 2, height / 2);

    int position = 0;
    createTrackbar("Position", "Video", &position, 100, onTrackbarSlide, &cap);

    while (true)
    {

        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;

        processImgFunc(frame);

        // Update the position of the trackbar
        setTrackbarPos("Position", "Video", (int) (cap.get(CAP_PROP_POS_FRAMES) / cap.get(CAP_PROP_FRAME_COUNT) * 100));

        // Check if the user has pressed the "Esc" key
        int key = waitKey(1000 / rate);
        if (key == 27)
            break;

        // Pause the video if the "Space" key is pressed
        if (key == 32)
        {
            while (true)
            {
                // Check if the "Space" key is released
                key = waitKey(10);
                if (key == 32)
                    break;
                if (key == 27)
                {
                    cap.release();
                    destroyWindow("Video");
                    return;
                }
            }
        }

        imshow("Video", frame);
    }

    cap.release();
    destroyWindow("Video");
}

void SimpleVideoPlayer::setImgProcessFunc(const function<void(Mat &)> &process_img_func)
{
    this->processImgFunc = process_img_func;
}

void SimpleVideoPlayer::defaultImgProcessFunc(Mat &img)
{
    // Pass
}
