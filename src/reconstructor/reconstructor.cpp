//
// Created by catslashbin on 22-11-27.
//

#include "reconstructor.h"
#include "utils/cv_utils.h"

using namespace cv;

void Reconstructor::reconstructArmors(std::vector<ArmorInfo>& armors, const RobotState& robot_state) {
    transformer.update(robot_state);
    for (auto &armor: armors) {
        armor.trans_model2cam = cam_calib.armorSolvePnP(armor.corners_model, armor.corners_img);
        armor.target_cam = opencvToRep(cvMat2Point3f(armor.trans_model2cam.tvec));
        armor.target_gimbal = eigenVecToCvPt3f(transformer.camToGimbal(cvPtToEigenVec3f(armor.target_cam)));
        armor.target_world = eigenVecToCvPt3f(transformer.gimbalToWorld(cvPtToEigenVec3f(armor.target_gimbal)));
    }
}

Point2f Reconstructor::cam2img(const Point3f &pt) {
    return cam_calib.projectToImage(pt);
}

void Reconstructor::solveAngle(const Point3f &target_world,
                               float *yaw_in_deg, float *horizontal_dist, float *vertical_dist) {
    Point3f target_gimbal = transformer.worldToGimbal(target_world);
    *horizontal_dist = sqrtf(powf(target_gimbal.x, 2) + powf(target_gimbal.y, 2));
    *vertical_dist = target_gimbal.z;
    *yaw_in_deg = atan2f(target_gimbal.y, target_gimbal.x);

    // assert(!(std::isnan(horizontal_dist) || std::isnan(vertical_dist) || std::isnan(yaw_in_deg)));
}

Reconstructor::Reconstructor(Config &cfg) : cam_calib(cfg), transformer(cfg) {}