//
// Created by catslashbin on 22-11-27.
//

#include "transformer.h"

using namespace cv;

void Transformer::reconstructArmor3D(DetectArmorInfo &armor) {
    armor.trans_model2cam = cam_calib.armorSolvePnP(armor.corners_model, armor.corners_img);
    armor.center_cam = mat2Point3f(armor.trans_model2cam.tvec);

    armor.center_base = armor.center_cam;
}

void Transformer::reconstructArmor3D(std::vector<DetectArmorInfo> &armors) {
    for (auto &armorInfo: armors) {
        reconstructArmor3D(armorInfo);
    }
}

Point2f Transformer::cam2img(const Point3f &pt) {
    return cam_calib.projectToImage({pt}).at(0);
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