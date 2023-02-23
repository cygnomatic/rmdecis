//
// Created by catslashbin on 22-11-27.
//

#include "reconstructor.h"
#include "utils/cv_utils.h"

using namespace cv;

void Reconstructor::reconstructArmor(FrameInput &frame_input) {
    transformer.update(frame_input.robot_state);
    for (auto &armor: frame_input.armor_info) {
        armor.trans_model2cam = cam_calib.armorSolvePnP(armor.corners_model, armor.corners_img);
        armor.target_cam = opencvToRep(cvMat2Point3f(armor.trans_model2cam.tvec));
        armor.target_gimbal = eigenVecToCvPt3f(transformer.camToGimbal(cvPtToEigenVec3f(armor.target_cam)));
        armor.target_world = eigenVecToCvPt3f(transformer.gimbalToWorld(cvPtToEigenVec3f(armor.target_gimbal)));
    }
}

Point2f Reconstructor::cam2img(const Point3f &pt) {
    return cam_calib.projectToImage(pt);
}

void Reconstructor::solveDistAndYaw(const Point3f &center_gimbal,
                                    float *yaw_in_deg, float *horizontal_dist, float *vertical_dist) {
    *horizontal_dist = sqrtf(powf(center_gimbal.x, 2) + powf(center_gimbal.y, 2));
    *vertical_dist = center_gimbal.z;
    *yaw_in_deg = atan2f(center_gimbal.y, center_gimbal.x);

    // assert(!(std::isnan(horizontal_dist) || std::isnan(vertical_dist) || std::isnan(yaw_in_deg)));
}

Reconstructor::Reconstructor(Config &cfg) : cam_calib(cfg), transformer(cfg) {}