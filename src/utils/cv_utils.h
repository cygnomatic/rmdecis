//
// Created by catslashbin on 23-1-3.
//

#ifndef CYGNOIDES_DECISION_CV_UTILS_H
#define CYGNOIDES_DECISION_CV_UTILS_H

#include "typing/core.h"

inline void drawArmorCorners(cv::Mat &image, ArmorCorners2d &corners,
                             const cv::Scalar &color = cv::Scalar(255, 0, 0), int thickness = 2) {
    cv::line(image, corners[0], corners[1], color, thickness);
    cv::line(image, corners[1], corners[2], color, thickness);
    cv::line(image, corners[2], corners[3], color, thickness);
    cv::line(image, corners[3], corners[0], color, thickness);
}

inline float calculateIoU(const cv::Rect2f &rect1, const cv::Rect2f &rect2) {
    cv::Rect2f intersect = rect1 & rect2;
    float intersectArea = intersect.area();
    float unionArea = rect1.area() + rect2.area() - intersectArea;
    float iou = intersectArea / unionArea;
    return std::isnan(iou) ? 0.f : iou;
}

inline cv::Point3f mat2Point3f(const cv::Mat &mat) {
    assert(mat.rows == 3 && mat.cols == 1);
    cv::Mat float_mat;
    mat.convertTo(float_mat, CV_32F);
    return {float_mat.at<float>(0), float_mat.at<float>(1), float_mat.at<float>(2)};
}

inline void drawPoint(cv::Mat &image, const cv::Point2f &point,
                      const cv::Scalar &color = cv::Scalar(255, 0, 0), int size = 5) {
    cv::circle(image, point, size, color, -1);
}

/**
 * Convert OpenCV coord to REP 103 coord represent.
 * @param pt OpenCV point
 * @return REP 103
 */
inline cv::Point3f opencvToRep(const cv::Point3f &pt) {
    return {pt.z, -pt.x, -pt.y};
}

#endif //CYGNOIDES_DECISION_CV_UTILS_H
