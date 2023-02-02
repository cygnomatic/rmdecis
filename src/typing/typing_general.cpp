//
// Created by catslashbin on 23-2-2.
//

#include "rmdecis/core.h"

ArmorCorners2d::ArmorCorners2d(cv::Point2f *corners) {
    tr = corners[0];
    tl = corners[1];
    dl = corners[2];
    dr = corners[3];
}

ArmorCorners2d::ArmorCorners2d(std::vector<cv::Point2f> corners) {
    assert(corners.size() == 4);
    tr = corners[0];
    tl = corners[1];
    dl = corners[2];
    dr = corners[3];
}

cv::Point2f &ArmorCorners2d::operator[](int index) {
    switch (index) {
        case 0:
            return tr;
        case 1:
            return tl;
        case 2:
            return dl;
        case 3:
            return dr;
        default:
            throw std::range_error("Corner index should be in 0 to 3.");
    }
}

ArmorCorners2d::operator std::vector<cv::Point2f>() const {
    return std::vector<cv::Point2f>({tr, tl, dl, dr});
}

ArmorCorners2d::operator cv::Rect2f() const {
    return getBoundingBox();
}

cv::Rect2f ArmorCorners2d::getBoundingBox() const {
    return cv::boundingRect(std::vector<cv::Point2f>({tr, tl, dl, dr}));
}

std::vector<cv::Point2f> ArmorCorners2d::toPts() {
    return std::vector<cv::Point2f>({tr, tl, dl, dr});
}

ArmorCorners3d::ArmorCorners3d(cv::Point3f *corners) {
    tr = corners[0];
    tl = corners[1];
    dl = corners[2];
    dr = corners[3];
}

ArmorCorners3d::ArmorCorners3d(std::vector<cv::Point3f> corners) {
    assert(corners.size() == 4);
    tr = corners[0];
    tl = corners[1];
    dl = corners[2];
    dr = corners[3];
}

ArmorCorners3d::ArmorCorners3d(const cv::Size_<float> &armor_size) :
        ArmorCorners3d(
                {{armor_size.width / 2,  armor_size.height / 2,  0},
                 {-armor_size.width / 2, armor_size.height / 2,  0},
                 {-armor_size.width / 2, -armor_size.height / 2, 0},
                 {armor_size.width / 2,  -armor_size.height / 2, 0}}
        ) {}

ArmorCorners3d::ArmorCorners3d(ArmorType armorType) {
    switch (armorType) {
        case SMALL_ARMOR:
            *this = ArmorCorners3d({SMALL_ARMOR_WIDTH, SMALL_ARMOR_HEIGHT});
        case BIG_ARMOR:
            *this = ArmorCorners3d({BIG_ARMOR_WIDTH, BIG_ARMOR_HEIGHT});
    }
}

cv::Point3f &ArmorCorners3d::operator[](int index) {
    switch (index) {
        case 0:
            return tr;
        case 1:
            return tl;
        case 2:
            return dl;
        case 3:
            return dr;
        default:
            throw std::range_error("Corner index should be in 0 to 3.");
    }
}

ArmorCorners3d::operator std::vector<cv::Point3f>() const {
    return std::vector<cv::Point3f>({tr, tl, dl, dr});
}

cv::Point3f ArmorCorners3d::getCenter() const {
    return (tr + tl + dl + dr) / 4;
}

DetectArmorInfo::DetectArmorInfo(FacilityID armor_type, const ArmorCorners2d &corners_img, float detection_confidence) :
        facility_id(armor_type), corners_img(corners_img), detection_confidence(detection_confidence) {
    corners_model = ArmorCorners3d{getArmorTypeFormID(facility_id)};
}
