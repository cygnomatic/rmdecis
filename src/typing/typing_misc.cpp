//
// Created by catslashbin on 23-2-2.
//

#include "rmdecis/core.h"
#include "typing.h"


Time::Time(int sec, int usec) : sec(sec), usec(usec), isDefined(true) {}

Time::Time() : sec(0), usec(0), isDefined(false) {}

float Time::operator-(const Time &rhs) const {
    if (isDefined && rhs.isDefined)
        return (sec - rhs.sec) + (usec - rhs.usec) * 1e-6f;
    return 0.f;
}

Time Time::operator+(const float rhs) const {
    int sec_ = sec + int(rhs);
    int usec_ = usec + int((rhs - int(rhs)) * 1e6);

    sec_ += usec_ / int(1e6);
    usec_ = usec_ % int(1e6);

    return Time{sec_, usec_};
}

EulerAngles::EulerAngles(const float *angles) {
    yaw = angles[0];
    pitch = angles[1];
    roll = angles[2];
}

EulerAngles::EulerAngles(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
    roll = 0.f;
}

Transform3d::Transform3d(cv::Mat rvec, cv::Mat tvec) {
    this->rvec = std::move(rvec);
    this->tvec = std::move(tvec);
}

std::vector<cv::Point3f> Transform3d::applyTo(const std::vector<cv::Point3f> &pts) const {
    cv::Mat rot_mat;
    std::vector<cv::Point3f> ret;
    Rodrigues(rvec, rot_mat);

    cv::transform(pts, ret, rot_mat);

    for (auto &pt: pts) {
        ret.push_back((cv::Point3f) tvec + pt);
    }

    return ret;
}

cv::Point3f Transform3d::applyTo(const cv::Point3f &pt) const {
    return applyTo(std::vector<cv::Point3f>{pt}).at(0);
}