//
// Created by catslashbin on 22-11-27.
//

#ifndef CYGNOIDES_DECISION_TRANSFORMER_H
#define CYGNOIDES_DECISION_TRANSFORMER_H

#include <utility>

#include "../typing/general.h"
#include "camera_calib.h"

struct SolveArmorResult
{
    float delta_depth, delta_height;
    float delta_yaw;
};

class Transformer
{
    Transform3d model2cam, cam2base;

    CameraCalib cam_calib;

    explicit Transformer(CameraCalib camera_calib): cam_calib(std::move(camera_calib)) {}


};


#endif //CYGNOIDES_DECISION_TRANSFORMER_H
