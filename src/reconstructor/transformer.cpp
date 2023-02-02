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

Transform::Transform(Frame &parent, Frame &child) : parent_(parent), child_(child) {}

Transform::Transform(Frame &parent, Frame &child, const Eigen::Matrix3f &rot, const Eigen::Vector3f &trans)
        : Transform(parent, child) {
    transform_.rotate(rot);
    transform_.translate(trans);
}

Transform::Transform(Frame &parent, Frame &child, const EulerAngles &rot, const Eigen::Vector3f& trans)
        : Transform(parent, child, cvtEulerToRotMat(rot), trans) {}

Eigen::Vector3f Transform::applyTo(const Eigen::Vector3f &pt) {
    return transform_ * pt;
}

Eigen::Vector3f Transform::applyInverseTo(const Eigen::Vector3f &pt) {
    return transform_.inverse(Eigen::Affine) * pt;
}
