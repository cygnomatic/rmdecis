//
// Created by catslashbin on 23-1-6.
//

#include "../utils_contrib/simple_video_player.h"
#include "../utils_contrib/simulate_vision_result.h"
#include "../src/tracker/armor_track.h"

int main()
{
    SimulateVisionOutput vision_output("../../data/vision_out/vision_result.yaml");
    SimpleVideoPlayer player("../../data/vision_out/video_input.avi");

    ArmorTrack armor_track{ArmorInfo{}};
    player.setPlaybackSpeed(0.5);

    int last_corr_t = 0;

    for (int i = 0;; i++)
    {
        Mat frame = player.getFrame();
        if (frame.empty())
            break;

        auto pred_result = vision_output.getData(player.frame_position);
        for (auto p: pred_result.armor_info)
        {
            // auto trans_model2cam = camera_calib.armorSolvePnP(ArmorCorners3d(SMALL_ARMOR), pred_result.corners_img_coord);
            //
            // String rot = fmt::format("Rvec [x: {:.2f}, y: {:.2f}, z: {:.2f}]", trans_model2cam.rvec.at<double>(0), trans_model2cam.rvec.at<double>(1), trans_model2cam.rvec.at<double>(2));
            // putText(frame, rot, {50, 200}, FONT_HERSHEY_SIMPLEX, 2, {255, 255, 255}, 3);
            //
            // auto reprojected_corners =  (ArmorCorners2d) camera_calib.projectToImage(std::vector<Point3f>(ArmorCorners3d(SMALL_ARMOR)), trans_model2cam);
            //
            // drawArmorCorners(frame, pred_result.corners_img_coord, {0, 0, 255});
            // drawArmorCorners(frame, reprojected_corners, {255, 0, 0});
            // camera_calib.drawAxes(frame, trans_model2cam);
            // break;

            ArmorInfo armor_info {p.armor_type, p.corners_img};

            drawArmorCorners(frame, p.corners_img, {0, 0, 255});
            rectangle(frame, p.corners_img.getBoundingBox(), {255, 0, 0}, 2);

            armor_track.correct(armor_info, i - last_corr_t);

            last_corr_t = i;

            break;
        }

        auto result_rect = armor_track.predict(i - last_corr_t);
        rectangle(frame, result_rect, {255, 0, 255}, 4);

        player.update(frame);
    }
}
