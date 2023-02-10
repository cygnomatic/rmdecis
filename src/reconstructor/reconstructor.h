//
// Created by catslashbin on 22-11-27.
//

#ifndef CYGNOIDES_DECISION_RECONSTRUCTOR_H
#define CYGNOIDES_DECISION_RECONSTRUCTOR_H

#include <utility>

#include "rmdecis/core.h"
#include "utils/cv_utils.h"
#include "reconstructor/camera_calib.h"
#include "reconstructor/transformer.h"

class Reconstructor {

    Transformer transformer;
    CameraCalib cam_calib;

public:

    explicit Reconstructor(Config& cfg);

    void reconstructArmor(FrameInput& frame_input);

    cv::Point2f cam2img(const cv::Point3f &pt);

    static void solveDistAndYaw(const cv::Point3f &center_gimbal, float &yaw_in_deg,
                                float &horizontal_dist, float &vertical_dist);
};


#endif //CYGNOIDES_DECISION_RECONSTRUCTOR_H
