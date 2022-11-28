//
// Created by catslashbin on 22-11-27.
//

#ifndef CYGNOIDES_DECISION_TRANSFORMER_H
#define CYGNOIDES_DECISION_TRANSFORMER_H

#include "../general/typing.h"
#include "camera_calib.h"

struct SolveArmorResult
{
    float delta_depth, delta_height;
    float delta_yaw;
};

class Transformer
{

    // static ArmorCorners3d reconstruct(CameraCalib &cam_calib,
    //                                   const ArmorCorners3d &corners_self_coord,
    //                                   const ArmorCorners2d &corners_img_coord);
    //
    // static ArmorCorners3d cam2gimbal_coord(ArmorCorners3d &corners_cam_coord, const Point3f &shift);
    //
    // static SolveArmorResult solve(ArmorCorners3d &corners_gimbal_coord);
    //
    // static SolveArmorResult solve(CameraCalib &cam_calib, const ArmorCorners3d &corners_self_coord,
    //                               const ArmorCorners2d &corners_img_coord, const Vec3f &shift);
};


#endif //CYGNOIDES_DECISION_TRANSFORMER_H
