#include "simple_video_player.h"

using namespace std;
using namespace cv;

void onTrackbarSlide(int position, void *userdata) {
    SimpleVideoPlayer *player = (SimpleVideoPlayer *) userdata;
    player->frame_position = (int) (position * player->cap.get(CAP_PROP_FRAME_COUNT) / 100.0);
    player->cap.set(CAP_PROP_POS_FRAMES, player->frame_position);
}

SimpleVideoPlayer::SimpleVideoPlayer(const string &path) {
    cap = VideoCapture(path);
    writer = VideoWriter();

    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the video file" << endl;
        return;
    }

    fps = cap.get(CAP_PROP_FPS);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);
    total_frames = cap.get(CAP_PROP_FRAME_COUNT);

    namedWindow("SimpleVideoPlayer", WINDOW_NORMAL);
    resizeWindow("SimpleVideoPlayer", width / 2, height / 2);

    createTrackbar("Position", "SimpleVideoPlayer", nullptr, 100, onTrackbarSlide, &cap);

    printInfo();
    cout << "[SimpleVideoPlayer] Video loaded. Start Playback." << endl;
}

SimpleVideoPlayer::~SimpleVideoPlayer() {
    closeStream();
}

void SimpleVideoPlayer::printInfo() const {
    cout << "[SimpleVideoPlayer] ====================================== " << endl;
    cout << "[SimpleVideoPlayer] Usage: " << endl;
    cout << "[SimpleVideoPlayer] \t- Press 'space' to pause the video. " << endl;
    cout << "[SimpleVideoPlayer] \t- Press 'esc' to exit. " << endl;
    cout << "[SimpleVideoPlayer] \t- Press 'r' to record. " << endl;
    cout << "[SimpleVideoPlayer] \t- Slide the Track bar to adjust playback process. " << endl;
    cout << "[SimpleVideoPlayer] Video Info: " << endl;
    cout << "[SimpleVideoPlayer] \t- Total Frame: " << total_frames << endl;
    cout << "[SimpleVideoPlayer] \t- Playback Speed: " << playback_speed << endl;
    cout << "[SimpleVideoPlayer] \t- Frame rate: " << fps << " fps" << endl;
    cout << "[SimpleVideoPlayer] \t- Playback Frame rate: " << fps * playback_speed << " fps" << endl;
    cout << "[SimpleVideoPlayer] \t- Frame size: " << width << " x " << height << endl;
    cout << "[SimpleVideoPlayer] ====================================== " << endl;
}

Mat SimpleVideoPlayer::getFrame() {
    Mat frame;
    cap >> frame;
    return frame;
}

Mat SimpleVideoPlayer::getFrame(int pos) {
    if (frame_position == pos) {
        return getFrame();
    } else {
        frame_position = pos;
        cap.set(CAP_PROP_POS_FRAMES, frame_position);
        return getFrame();
    }
}

void SimpleVideoPlayer::update(Mat &frame) {

    // Update the track_bar_position of the trackbar
    setTrackbarPos("Position", "SimpleVideoPlayer",
                   (int) (cap.get(CAP_PROP_POS_FRAMES) / cap.get(CAP_PROP_FRAME_COUNT) * 100));

    // Update timing
    if (is_timing) {
        timing_idx++;
        if (timing_idx % 10 == 0) {
            auto timing_now = chrono::high_resolution_clock::now();
            chrono::duration<float, std::milli> dur_ms = timing_now - timing_last;
            fps_display =
                    std::to_string(10 / (dur_ms.count() / 1000)) + " / " + std::to_string(fps * playback_speed);

            timing_last = timing_now;
        }
        putText(frame, "FPS: " + fps_display, {width - 600, height - 200}, FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 1);
    }

    int key = waitKey(1000 / fps / playback_speed);

    // std::cout << key << endl;

    switch (key) {
        case 27: // "Esc"
            closeStream();
            break;
        case 114: // "R"
            toggleRecording();
            break;
        case 116: // "T"
            toggleTiming();
            break;
        case 32: // "Space"
            while (true) {
                // Check if the "Space" key is released
                key = waitKey(10);
                if (key == 32)
                    break;
                if (key == 27) {
                    closeStream();
                    return;
                }
            }
            break;
        default:
            break;
    }

    if (is_recording) {
        writer.write(frame);
        putText(frame, "RECORDING...", {width - 500, 200}, FONT_HERSHEY_SIMPLEX, 2, {0, 0, 255}, 6);
    }

    imshow("SimpleVideoPlayer", frame);
    frame_position++;
}

void SimpleVideoPlayer::closeStream() {
    cap.release();
    writer.release();
    destroyWindow("SimpleVideoPlayer");
}

void SimpleVideoPlayer::setPlaybackSpeed(float speed) {
    this->playback_speed = speed;
}

void SimpleVideoPlayer::toggleRecording() {
    if (is_recording) {
        // Stop recording.
        is_recording = false;

        cout << "[SimpleVideoPlayer] Stop Recording." << endl;
        writer.release();

    } else {
        // Start recording.
        is_recording = true;

        // Get time.
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "REC-%Y%m%d-%H%M%S.mp4");
        auto filename = oss.str();

        cout << "[SimpleVideoPlayer] Start Recording. Record file: " + filename << endl;
        writer = VideoWriter(filename, VideoWriter::fourcc('m', 'p', '4', 'v'), fps * playback_speed * record_speed,
                             {width, height});
    }
}

void SimpleVideoPlayer::toggleTiming() {
    is_timing = !is_timing;
    timing_idx = 0;
    timing_last = chrono::high_resolution_clock::now();
}

void SimpleVideoPlayer::setRecordSpeed(float speed) {
    record_speed = speed;
}
