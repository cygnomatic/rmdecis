//
// Created by catslashbin on 23-1-13.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "rmdecis/core.h"
#include "utils/preprocess.h"

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

        tracker.update(armor_infos, detect_result.seq_idx);

        for (auto &t: detect_result.armor_info) {
            drawArmorCorners(frame, t.corners_img, {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {

            track_info = p.second.predict(detect_result.seq_idx + 10);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 100, 100}, 3);

            track_info = p.second.predict(detect_result.seq_idx + 5);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 150, 150}, 3);

            track_info = p.second.predict(detect_result.seq_idx);
            drawPoint(frame, reconstructor.cam2img(reconstructor.transformer.worldToCam(track_info.target_world)),
                      {0, 250, 250}, 3);
        }

        player.update(frame);
    }
}
