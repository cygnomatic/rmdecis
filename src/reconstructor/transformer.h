//
// Created by catslashbin on 22-11-27.
//

#ifndef CYGNOIDES_DECISION_TRANSFORMER_H
#define CYGNOIDES_DECISION_TRANSFORMER_H

#include <utility>

#include "typing/core.h"
#include "utils/cv_utils.h"
#include "reconstructor/camera_calib.h"

struct SolveArmorResult {
    float delta_depth, delta_height;
    float delta_yaw;
};

class Transformer {

    Transform3d trans_model2cam, trans_cam2base;

    CameraCalib cam_calib;

public:

    void reconstructArmor3D(DetectArmorInfo &armor);

    void reconstructArmor3D(std::vector<DetectArmorInfo> &armors);

    explicit Transformer(CameraCalib camera_calib) : cam_calib(std::move(camera_calib)) {}

    cv::Point2f cam2img(const cv::Point3f &pt);
};


#endif //CYGNOIDES_DECISION_TRANSFORMER_H
