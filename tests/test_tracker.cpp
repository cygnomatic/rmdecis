//
// Created by catslashbin on 23-1-13.
//

//
// Created by catslashbin on 23-1-6.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "tracker/tracker.h"
#include "reconstructor/camera_calib.h"

using cv::Mat;
using cv::rectangle;

int main() {

    set_level(spdlog::level::debug);

    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");
    CameraCalib camera_calib("../../config/cam_cali_coeffs.yml");

    player.setPlaybackSpeed(1);

    Tracker tracker{0.75, 4, 3};
    cv::Rect2f bbox;

    for (int i = 0;; i++) {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto pred_result = vision_output.getData(player.frame_position);
        auto reconstruct_result = ReconstructArmorResult(pred_result);

        for (auto &a: pred_result.armor_info) {
            auto trans = camera_calib.armorSolvePnP(ArmorCorners3d{SMALL_ARMOR}, a.corners_img);
            cv::Point3f c{(float) trans.tvec.at<double>(0), (float) trans.tvec.at<double>(1),
                          (float) trans.tvec.at<double>(2)};
            reconstruct_result.armor_info.emplace_back(a, c, trans.rvec);
        }

        tracker.update(reconstruct_result);


        for (auto &t: pred_result.armor_info) {
            rectangle(frame, t.corners_img.getBoundingBox(), {255, 255, 0}, 2);
        }

        for (auto &p: tracker.getTracks()) {


            bbox = p.second.predict(pred_result.time).bbox;
            rectangle(frame, bbox, {0, 255, 255}, 5);

            bbox = p.second.predict(pred_result.time + 5).bbox;
            rectangle(frame, bbox, {0, 150, 150}, 5);

            bbox = p.second.predict(pred_result.time + 10).bbox;
            rectangle(frame, bbox, {0, 50, 50}, 5);

            debug("i={}, bbox.x={}, bbox.y={}", i, bbox.x, bbox.y);
        }

        player.update(frame);
    }
}
