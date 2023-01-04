#include "simple_video_player.h"

void onTrackbarSlide(int position, void *userdata)
{
    SimpleVideoPlayer *player = (SimpleVideoPlayer *) userdata;
    player->frame_position = (int) (position * player->cap.get(CAP_PROP_FRAME_COUNT) / 100.0);
    player->cap.set(CAP_PROP_POS_FRAMES, player->frame_position);
}

SimpleVideoPlayer::SimpleVideoPlayer(const String& path)
{
    cap = VideoCapture(path);
    if (!cap.isOpened())
    {
        cerr << "ERROR: Unable to open the video file" << endl;
        return;
    }

    fps = cap.get(CAP_PROP_FPS);
    cout << "Frame rate: " << fps << " fps" << endl;

    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "Frame size: " << width << " x " << height << endl;

    namedWindow("Video", WINDOW_NORMAL);
    resizeWindow("Video", width / 2, height / 2);

    createTrackbar("Position", "Video", &track_bar_position, 100, onTrackbarSlide, &cap);
}

SimpleVideoPlayer::~SimpleVideoPlayer()
{
    closeStream();
}

Mat SimpleVideoPlayer::getFrame()
{
    Mat frame;
    cap >> frame;
    return frame;
}

void SimpleVideoPlayer::update(Mat &frame)
{

    // Update the track_bar_position of the trackbar
    setTrackbarPos("Position", "Video", (int) (cap.get(CAP_PROP_POS_FRAMES) / cap.get(CAP_PROP_FRAME_COUNT) * 100));

    // Check if the user has pressed the "Esc" key
    int key = waitKey(1000 / fps);
    if (key == 27)
        closeStream();

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
                closeStream();
                return;
            }
        }
    }

    imshow("Video", frame);

}

void SimpleVideoPlayer::closeStream()
{
    cap.release();
    destroyWindow("Video");
}
