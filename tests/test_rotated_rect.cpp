//
// Created by catslashbin on 23-2-18.
//

#include "rmdecis_impl/basic_aiming_impl.h"
#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"

using namespace cv;

int main() {

    spdlog::set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../data/vision_out/vision_result_1.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input_1.avi");

    Config cfg("../config/config.yml");

    while (true) {

        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        ArmorFrameInput detection = vision_output.getData(player.frame_position);

        for (auto &t: detection.armor_info) {
            std::vector<Point2f> pts(4);
            minAreaRect((std::vector<Point2f>)t.corners_img).points(pts.data());
            drawPolygons(frame, pts, {255, 0, 0}, 5);
        }

        player.update(frame);
    }
}
