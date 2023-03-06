//
// Created by catslashbin on 23-1-24.
//

#include "large_rune_aiming_impl.h"

using namespace cv;
using namespace Eigen;

EulerAngles LargeRuneAiming::LargeRuneAimingImpl::update(RuneFrameInput frame_input) {

    if (frame_input.vane_info.empty())
        return last_aiming_angle_;

    assert(frame_input.vane_info.size() == 1);
    VaneInfo armor = VaneInfo(frame_input.vane_info[0]);

    // Reconstruct
    rune_reconstructor.reconstructVanes({armor}, frame_input.robot_state);

    // Update rune info
    normal_vec_world_ = (center_pt_world_ * n_updates +
            (armor.normal_pt_world - armor.center_pt_world.normalized())).normalized();
    center_pt_world_ = (center_pt_world_ * n_updates + armor.center_pt_world) / (n_updates + 1);
    n_updates++;

    // Get angle
    float curr_angle = rune_reconstructor.getRuneAngle(armor, normal_vec_world_, center_pt_world_);
    debug("Target vane angle: {}", curr_angle);
}



LargeRuneAiming::LargeRuneAimingImpl::LargeRuneAimingImpl(Config &cfg)
        : rune_reconstructor(cfg), tracker(cfg),
          compensator(cfg.get<float>("aiming.basic.airResistanceConst", 0.1)) {
}