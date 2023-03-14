//
// Created by catslashbin on 23-2-25.
//

#include "rune_reconstructor.h"

using namespace Eigen;

RuneReconstructor::RuneReconstructor(Config &cfg) : cam_calib(cfg), transformer(cfg) {}

void RuneReconstructor::reconstructVanes(std::vector<VaneInfo> vanes, const RobotState &robot_state) {
    transformer.update(robot_state);
    for (auto &vane: vanes) {


        vane.center_pt_cam = Transformer::modelToCam(cvPtToEigenVec3f(vane.corners_model[0]),
                                                     vane.trans_model2cam);
        vane.center_pt_world = transformer.camToWorld(vane.center_pt_cam);
        vane.target_pt_cam = Transformer::modelToCam(Vector3f(0.0, 0.0, 0.0), vane.trans_model2cam);
        vane.target_pt_world = transformer.camToWorld(vane.target_pt_cam);

        vane.normal_pt_cam = Transformer::modelToCam(Vector3f(0.0, 0.0, 1.0), vane.trans_model2cam);

        // Ensure normal-vector points towards the cam
        vane.normal_pt_cam = ((vane.normal_pt_cam - vane.center_pt_cam).x() > 0) ?
                             vane.normal_pt_cam :
                             Vector3f(2 * vane.center_pt_cam - vane.normal_pt_cam);

        vane.normal_pt_world = transformer.camToWorld(vane.normal_pt_cam);
    }
}

float RuneReconstructor::getRuneAngle(const VaneInfo& vane, const Vector3f& normal_vec_world, const Vector3f& center_pt_world) {

    // Project "up" vector into the rune plane
    Vector3f up_vec_world = Vector3f(0, 0, 1.0);
    Vector3f up_error_vec_world = normal_vec_world * (normal_vec_world.dot(up_vec_world));
    Vector3f up_vec_plane_world = (up_vec_world - up_error_vec_world).normalized();

    // Project target vector into the rune plane
    Vector3f target_vec_world = vane.target_pt_world - center_pt_world;
    Vector3f target_error_vec_world = normal_vec_world * (normal_vec_world.dot(target_vec_world));
    Vector3f target_vec_plane_world = (target_vec_world - target_error_vec_world).normalized();

    // Calculate angle in range(0, 360) between vector "up" & "target"
    // Ref: https://math.stackexchange.com/questions/878785/how-to-find-an-angle-in-range0-360-between-2-vectors
    float dot = up_vec_plane_world.dot(target_vec_plane_world);
    float det = normal_vec_world.dot(up_vec_plane_world.cross(target_vec_plane_world)); // Triple product

    return atan2(det, dot);
}