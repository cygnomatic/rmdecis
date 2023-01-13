//
// Created by catslashbin on 23-1-13.
//

//
// Created by catslashbin on 23-1-6.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/tracker/armor_track.h"

using namespace cv;

int main() {
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");

    player.setPlaybackSpeed(0.1);

    int last_corr_t = 0;

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto pred_result = vision_output.getData(player.frame_position);
        for (auto p: pred_result.armor_info) {

            drawArmorCorners(frame, p.corners_img, {0, 0, 255});
            rectangle(frame, p.corners_img.getBoundingBox(), {255, 0, 0}, 2);

            armor_track.correct(p, i - last_corr_t);

            last_corr_t = i;

            break;
        }

        auto result_rect = armor_track.predict(i - last_corr_t);
        rectangle(frame, result_rect, {255, 0, 255}, 4);

        player.update(frame);
    }
}
