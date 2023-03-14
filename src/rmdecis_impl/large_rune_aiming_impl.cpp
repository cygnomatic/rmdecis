//
// Created by catslashbin on 23-1-24.
//

#include "large_rune_aiming_impl.h"
#include "utils/cv_utils.h"

#define DEBUG if (debug_frame != nullptr && !debug_frame->empty())

using namespace cv;
using namespace Eigen;

Point2f world2img(const Vector3f &pt, RuneReconstructor &reconstructor) {
    return reconstructor.cam_calib.projectToImage(eigenVecToCvPt3f(reconstructor.transformer.worldToCam(pt)));
}

EulerAngles LargeRuneAiming::LargeRuneAimingImpl::update(RuneFrameInput frame_input, cv::Mat *debug_frame) {

    if (frame_input.vane_info.empty())
        return last_aiming_angle_;

    assert(frame_input.vane_info.size() == 1);
    VaneInfo armor = VaneInfo(frame_input.vane_info[0]);

    // Reconstruct
    rune_reconstructor.reconstructVanes({armor}, frame_input.robot_state);

    // Update rune info
    Vector3f normal_vec_world = armor.normal_pt_world - armor.center_pt_world.normalized();
    normal_vec_world_ = (center_pt_world_ * n_updates + normal_vec_world).normalized();
    center_pt_world_ = (center_pt_world_ * n_updates + armor.center_pt_world) / (n_updates + 1);
    vane_length_ = (vane_length_ * n_updates + (armor.target_pt_world - center_pt_world_).norm()) / (n_updates + 1);
    n_updates++;

    // Get angle
    float curr_angle = rune_reconstructor.getRuneAngle(armor, normal_vec_world_, center_pt_world_);
    debug("Target vane angle: {}", curr_angle);

    DEBUG {
        arrowedLine(*debug_frame,
                    world2img(armor.center_pt_world, rune_reconstructor),
                    world2img(armor.target_pt_world, rune_reconstructor),
                    {255, 255, 255}, 3);
    }

    return last_aiming_angle_;
}


LargeRuneAiming::LargeRuneAimingImpl::LargeRuneAimingImpl(Config &cfg)
        : rune_reconstructor(cfg), tracker(cfg),
          compensator(cfg.get<float>("aiming.basic.airResistanceConst", 0.1)) {
}