//
// Created by catslashbin on 23-1-4.
//


#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "rmdecis/core.h"
#include "utils/preprocess.h"
#include "reconstructor/transformer.h"

using cv::Mat;
using cv::rectangle;

int main() {

    spdlog::set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../data/vision_out/video_input.mp4");

    Config cfg("../config/config.yml");
    Reconstructor reconstructor(cfg);

    player.setPlaybackSpeed(1);
    player.setRecordSpeed(2);

    Tracker tracker(cfg);
    TrackArmorInfo track_info;

    int width = cfg.get<int>("camera.width"), height = cfg.get<int>("camera.height");

    while (true) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto detect_result = vision_output.getData(player.frame_position);

        std::vector<ArmorInfo> armor_infos = detectionToInfo(detect_result.armor_info, width, height,
                                                             0.6, CompetitionRule(cfg));

        reconstructor.reconstructArmors(armor_infos, detect_result.robot_state);

        for (auto &a: armor_infos) {
            drawArmorCorners(frame, a.corners_img, {255, 255, 0}, 1);
            std::vector<cv::Point3f> corners_cam;
            for (auto &c: std::vector<cv::Point3f>(a.corners_model)) {
                corners_cam.push_back(Transformer::modelToCam(c, a.trans_model2cam));
            }
            std::vector<cv::Point2f> corners_img_reproj = a.reconstructor->cam_calib.projectToImage(corners_cam);
            drawPolygons(frame, corners_img_reproj, {0, 255, 255}, 1);
        }

        player.update(frame);
    }
}
