//
// Created by catslashbin on 23-1-24.
//

#include "large_rune_aiming_impl.h"

using namespace cv;
using namespace Eigen;

EulerAngles LargeRuneAiming::LargeRuneAimingImpl::update(ArmorFrameInput frame_input) {

    if (frame_input.armor_info.empty())
        return last_aiming_angle_;

    assert(frame_input.armor_info.size() == 1);
    ArmorInfo armor = ArmorInfo(frame_input.armor_info[0]);

    // Reconstruct
    transformer.update(frame_input.robot_state);
    CvTransform3f trans_model2cam = camera_calib.solvePnP(armor.corners_model, armor.corners_img);

    // Transform
    Vector3f center_pt_cam = Transformer::modelToCam(cvPtToEigenVec3f(armor.corners_model[0]), armor.trans_model2cam);
    Vector3f center_pt_world = transformer.camToWorld(center_pt_cam);
    Vector3f target_pt_cam = Transformer::modelToCam(Vector3f(0.0, 0.0, 0.0), armor.trans_model2cam);
    Vector3f target_pt_world = transformer.camToWorld(target_pt_cam);

    Vector3f normal_pt_cam = Transformer::modelToCam(Vector3f(0.0, 0.0, 1.0), armor.trans_model2cam);
    normal_pt_cam = ((normal_pt_cam - center_pt_cam).x() > 0) ?
                    normal_pt_cam :
                    Vector3f(2 * center_pt_cam - normal_pt_cam); // Ensure normal-vector points towards the cam
    Vector3f normal_pt_world = transformer.camToWorld(normal_pt_cam);

    Vector3f normal_vec_world = (normal_pt_world - center_pt_world).normalized();

    // Update rune info
    normal_vec_world_ = (center_pt_world_ * n_updates + normal_vec_world).normalized();
    center_pt_world_ = (center_pt_world_ * n_updates + center_pt_world) / (n_updates + 1);

    // Project "up" vector into the rune plane
    Vector3f up_vec_world = Vector3f(0, 0, 1.0);
    Vector3f up_error_vec_world = normal_vec_world_ * (normal_vec_world_.dot(up_vec_world));
    Vector3f up_vec_plane_world = (up_vec_world - up_error_vec_world).normalized();

    // Project target vector into the rune plane
    Vector3f target_vec_world = target_pt_world - center_pt_world_;
    Vector3f target_error_vec_world = normal_vec_world_ * (normal_vec_world_.dot(target_vec_world));
    Vector3f target_vec_plane_world = (target_vec_world - target_error_vec_world).normalized();

    // Calculate angle in range(0, 360) between vector "up" & "target"
    // Ref: https://math.stackexchange.com/questions/878785/how-to-find-an-angle-in-range0-360-between-2-vectors
    float dot = up_vec_plane_world.dot(target_vec_plane_world);
    float det = normal_vec_world_.dot(up_vec_plane_world.cross(target_vec_plane_world)); // Triple product
    float angle = atan2(det, dot);

}


LargeRuneAiming::LargeRuneAimingImpl::LargeRuneAimingImpl(Config &cfg)
        : transformer(cfg), camera_calib(cfg), tracker(cfg),
          compensator(cfg.get<float>("aiming.basic.airResistanceConst", 0.1)) {
}