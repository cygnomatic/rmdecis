//
// Created by catslashbin on 23-2-2.
//

#include <utility>

#include "rmdecis/core.h"
#include "typing.h"


Corners2fImpl::Corners2fImpl(std::vector<cv::Point2f> corners) : corners_(std::move(corners)) {}

cv::Point2f &Corners2fImpl::operator[](int index) {
    return corners_[index];
}

Corners2fImpl::operator std::vector<cv::Point2f>() const {
    return corners_;
}

// cv::Rect2f BaseCorners2f::getBoundingBox() const {
//     throw std::logic_error("Function not yet implemented");
// }

size_t Corners2fImpl::size() {
    return corners_.size();
}

cv::Rect2f RectCorners2f::getBoundingBox() const {
    return cv::boundingRect(corners_);
}

RectCorners2f::RectCorners2f(cv::Point2f tr, cv::Point2f tl, cv::Point2f dl, cv::Point2f dr) :
        RectCorners2f({std::move(tr), std::move(tl), std::move(dl), std::move(dr)}) {}

RuneCorners2f::RuneCorners2f(cv::Point2f center, cv::Point2f dr, cv::Point2f tr,
                             cv::Point2f top, cv::Point2f tl, cv::Point2f dl):
        RuneCorners2f({std::move(center),std::move(dr), std::move(tr),
                       std::move(top), std::move(tl), std::move(dl)}) {}

cv::Rect2f RuneCorners2f::getBoundingBox() const {
    // Use only four points on the rune armor corners
    return cv::boundingRect(std::vector<cv::Point2f>{corners_[1], corners_[2], corners_[4], corners_[5]});
}

Corners2f::Corners2f(RectCorners2f corners) {
    impl = std::make_shared<RectCorners2f>(std::move(corners));
}

Corners2f::Corners2f(RuneCorners2f corners) {
    impl = std::make_shared<RuneCorners2f>(std::move(corners));
}

cv::Point2f &Corners2f::operator[](int index) {
    return impl->operator[](index);
}

size_t Corners2f::size() {
    return impl->size();
}

cv::Rect2f Corners2f::getBoundingBox() const {
    return impl->getBoundingBox();
}

Corners2f::operator std::vector<cv::Point2f>() const {
    return impl->operator std::vector<cv::Point2f>();
}

Corners3fImpl::Corners3fImpl(std::vector<cv::Point3f> corners) : corners_(std::move(corners)) {}

cv::Point3f &Corners3fImpl::operator[](int index) {
    return corners_[index];
}

Corners3fImpl::operator std::vector<cv::Point3f>() const {
    return corners_;
}

RectCorners3f::RectCorners3f(const cv::Size_<float> &armor_size) :
        RectCorners3f(
                {{armor_size.width / 2,  armor_size.height / 2,  0},
                 {-armor_size.width / 2, armor_size.height / 2,  0},
                 {-armor_size.width / 2, -armor_size.height / 2, 0},
                 {armor_size.width / 2,  -armor_size.height / 2, 0}}
        ) {}

SmallRectCorners3f::SmallRectCorners3f() : SmallRectCorners3f(cv::Size(SMALL_ARMOR_WIDTH, SMALL_ARMOR_HEIGHT)) {}

LargeRectCorners3f::LargeRectCorners3f() : LargeRectCorners3f(cv::Size(BIG_ARMOR_WIDTH, BIG_ARMOR_HEIGHT)) {}

RuneCorners3f::RuneCorners3f() {
    throw std::logic_error("Function not yet implemented");
}

Corners3f::Corners3f(SmallRectCorners3f corners) {
    impl = std::make_shared<SmallRectCorners3f>(std::move(corners));
}

Corners3f::Corners3f(LargeRectCorners3f corners) {
    impl = std::make_shared<LargeRectCorners3f>(std::move(corners));
}

Corners3f::Corners3f(RuneCorners3f corners) {
    impl = std::make_shared<RuneCorners3f>(std::move(corners));
}

cv::Point3f &Corners3f::operator[](int index) {
    return impl->operator[](index);
}

Corners3f::operator std::vector<cv::Point3f>() const {
    return impl->operator std::vector<cv::Point3f>();
}


DetectArmorInfo::DetectArmorInfo(FacilityID armor_type, Corners2f corners_img,
                                 float detection_confidence) :
        facility_id(armor_type), corners_img(std::move(corners_img)), detection_confidence(detection_confidence) {
    corners_model = SmallRectCorners3f();
}

FrameInput::FrameInput(int seq_idx, Time time, std::vector<DetectArmorInfo> armor_info, RobotState robot_state) :
        seq_idx(seq_idx), time(time), armor_info(std::move(armor_info)), robot_state(robot_state) {}


RobotState::RobotState(float gimbal_yaw, float gimbal_pitch, float ballet_init_speed) :
        gimbal_pitch(gimbal_pitch), gimbal_yaw(gimbal_yaw), ballet_init_speed(ballet_init_speed) {}
