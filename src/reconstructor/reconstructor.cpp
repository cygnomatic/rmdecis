//
// Created by catslashbin on 22-11-27.
//

#include "reconstructor.h"

using namespace cv;

void Reconstructor::reconstructArmor3D(DetectArmorInfo &armor) {
    armor.trans_model2cam = cam_calib.armorSolvePnP(armor.corners_model, armor.corners_img);
    armor.center_cam = mat2Point3f(armor.trans_model2cam.tvec);

    armor.center_base = armor.center_cam;
}

void Reconstructor::reconstructArmor3D(std::vector<DetectArmorInfo> &armors) {
    for (auto &armorInfo: armors) {
        reconstructArmor3D(armorInfo);
    }
}

Point2f Reconstructor::cam2img(const Point3f &pt) {
    return cam_calib.projectToImage({pt}).at(0);
}

void Reconstructor::solveDistAndYaw(const Point3f& center_gimbal, float& yaw_in_deg, float &horizontal_dist, float &vertical_dist) {
    horizontal_dist = sqrtf(powf(center_gimbal.x, 2) + powf(center_gimbal.y, 2));
    vertical_dist = center_gimbal.z;
    yaw_in_deg = atan2f(center_gimbal.y, center_gimbal.x);

    assert(!(std::isnan(horizontal_dist) || std::isnan(vertical_dist) || std::isnan(yaw_in_deg)));
}

// std::vector<Point3f> Transformer::modelToCam(const std::vector<Point3f>& pts_model)
// {
//
// }
//
// std::vector<Point2f> Transformer::modelToImg(const std::vector<Point3f>& pts_model)
// {
//
// }