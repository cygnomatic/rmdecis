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
public:

    Transformer transformer;
    CameraCalib cam_calib;

    explicit Reconstructor(Config& cfg);

    void reconstructArmors(std::vector<ArmorInfo> &armors, const RobotState &robot_state);

    cv::Point2f cam2img(const cv::Point3f &pt);

    void solveAngle(const cv::Point3f &target_world, float *yaw_in_deg,
                           float *horizontal_dist, float *vertical_dist);
};


#endif //CYGNOIDES_DECISION_RECONSTRUCTOR_H
