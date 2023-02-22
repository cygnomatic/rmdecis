//
// Created by catslashbin on 23-1-31.
//

#ifndef CYGNOIDES_DECISION_TRANSFORMER_H
#define CYGNOIDES_DECISION_TRANSFORMER_H

#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>

#include "rmdecis/core.h"
#include "utils/cv_utils.h"

class Transform {
private:

    Eigen::Affine3f transform_;

public:

    Transform();

    Transform(const Eigen::Matrix3f &rot, const Eigen::Vector3f &trans);

    Transform(const EulerAngles &rot, const Eigen::Vector3f &trans);

    Transform(Config &cfg, std::string field);

    Eigen::Vector3f applyTo(const Eigen::Vector3f &pt);

    Eigen::Vector3f applyInverseTo(const Eigen::Vector3f &pt);
};

class Transformer {
public:

    void update(const RobotState &robot_state);

    cv::Point3f camToGimbal(const cv::Point3f &pt);

    cv::Point3f gimbalToWorld(const cv::Point3f &pt);

    cv::Point3f camToWorld(const cv::Point3f &pt);

    cv::Point3f worldToGimbal(const cv::Point3f &pt);

    explicit Transformer(Config &cfg);

private:
    Transform trans_cam2gt_;
    Transform trans_gt2gimbal_;
    Transform trans_gimbal2world_;
};


#endif //CYGNOIDES_DECISION_TRANSFORMER_H
