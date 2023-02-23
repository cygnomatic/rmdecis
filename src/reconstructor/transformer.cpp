//
// Created by catslashbin on 23-1-31.
//

#include "transformer.h"

using cv::Point3f;
using Eigen::Matrix3f;

/**
 * Convert intrinsic euler angle To Right-hand Rotation Matrix.
 * @param euler euler angle, Intrinsic, {z,y',x'}
 * @return Rotation Matrix
 * @see https://dominicplein.medium.com/extrinsic-intrinsic-rotation-do-i-multiply-from-right-or-left-357c38c1abfd
 */
static Matrix3f cvtEulerToRotMat(const EulerAngles &euler) {

    Eigen::AngleAxisf rz(euler.yaw, Eigen::Vector3f::UnitZ());
    Eigen::AngleAxisf ry(euler.pitch, Eigen::Vector3f::UnitY());
    Eigen::AngleAxisf rx(euler.roll, Eigen::Vector3f::UnitX());

    Eigen::Quaternionf q = rz * ry * rx;
    return q.matrix();
}

Transform::Transform(const Eigen::Matrix3f &rot, const Eigen::Vector3f &trans) {
    transform_.setIdentity();
    transform_.rotate(rot);
    transform_.pretranslate(trans);
}

Transform::Transform(const EulerAngles &rot, const Eigen::Vector3f &trans)
        : Transform(cvtEulerToRotMat(rot), trans) {}

Transform::Transform(Config &cfg, std::string field) {
    auto rot = EulerAngles(cfg.get<float>(field + ".rot.yaw"),
                           cfg.get<float>(field + ".rot.pitch"),
                           cfg.get<float>(field + ".rot.roll"));
    auto trans = Eigen::Vector3f(cfg.get<float>(field + ".trans.x"),
                                 cfg.get<float>(field + ".trans.y"),
                                 cfg.get<float>(field + ".trans.z"));
    transform_.setIdentity();
    transform_.rotate(cvtEulerToRotMat(rot));
    transform_.pretranslate(trans);
}

Eigen::Vector3f Transform::applyTo(const Eigen::Vector3f &pt) {
    return transform_ * pt;
}

Eigen::Vector3f Transform::applyInverseTo(const Eigen::Vector3f &pt) {
    return transform_.inverse(Eigen::Affine) * pt;
}

Transform::Transform() = default;

Transformer::Transformer(Config &cfg) {
    trans_cam2gt_ = Transform(cfg, "transformer.camToGimbalT");
}

void Transformer::update(const RobotState &robot_state) {
    trans_gt2gimbal_ = Transform(EulerAngles(0, -robot_state.gimbal_pitch, 0), Eigen::Vector3f(0, 0, 0));
    trans_gimbal2world_ = Transform(EulerAngles(-robot_state.gimbal_yaw, 0, 0), Eigen::Vector3f(0, 0, 0));
}

Eigen::Vector3f Transformer::camToGimbal(const Eigen::Vector3f &pt) {
    // OpenCV Point3f to Eigen Vector3f
    return trans_gt2gimbal_.applyTo(trans_cam2gt_.applyTo(pt));
}

Eigen::Vector3f Transformer::gimbalToWorld(const Eigen::Vector3f &pt) {
    return trans_gimbal2world_.applyTo(pt);
}

Eigen::Vector3f Transformer::camToWorld(const Eigen::Vector3f &pt) {
    return trans_gimbal2world_.applyTo(
            trans_gt2gimbal_.applyTo(trans_cam2gt_.applyTo(pt))
    );
}

Eigen::Vector3f Transformer::worldToGimbal(const Eigen::Vector3f &pt) {
    return trans_gimbal2world_.applyInverseTo(pt);
}

cv::Point3f Transformer::camToGimbal(const Point3f &pt) {
    throw std::logic_error("Method not implement yet!");
}

cv::Point3f Transformer::gimbalToWorld(const Point3f &pt) {
    throw std::logic_error("Method not implement yet!");
}

cv::Point3f Transformer::camToWorld(const Point3f &pt) {
    throw std::logic_error("Method not implement yet!");
}

cv::Point3f Transformer::worldToGimbal(const Point3f &pt) {
    throw std::logic_error("Method not implement yet!");
}

cv::Point3f Transformer::modelToCam(const Point3f &pt, const CvTransform3f &trans_model2cam) {
    // OpenCV has use different camera frame standard from REP. Convert it to REP103 standard.
    return opencvToRep(trans_model2cam.applyTo(pt));
}
