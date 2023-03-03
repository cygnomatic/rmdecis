//
// Created by catslashbin on 23-1-3.
//

#ifndef CYGNOIDES_DECISION_CV_UTILS_H
#define CYGNOIDES_DECISION_CV_UTILS_H

#include "rmdecis/core.h"
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>

inline void drawArmorCorners(cv::Mat &image, Corners2f &corners,
                             const cv::Scalar &color = cv::Scalar(255, 0, 0), int thickness = 2) {
    for (int i = 0; i < corners.size() - 1; i++) {
        cv::line(image, corners[i], corners[i + 1], color, thickness);
    }
    cv::line(image, corners[corners.size() - 1], corners[0], color, thickness);
}

inline void drawPolygons(cv::Mat &image, std::vector<cv::Point2f> &pts,
                             const cv::Scalar &color = cv::Scalar(255, 0, 0), int thickness = 2) {
    for (int i = 0; i < pts.size() - 1; i++) {
        cv::line(image, pts[i], pts[i + 1], color, thickness);
    }
    cv::line(image, pts[pts.size() - 1], pts[0], color, thickness);
}

inline void drawPolygons(cv::Mat &image, cv::RotatedRect rect,
                             const cv::Scalar &color = cv::Scalar(255, 0, 0), int thickness = 2) {
    std::vector<cv::Point2f> pts(4);
    rect.points(pts.data());
    drawPolygons(image, pts, color, thickness);
}

inline float calculateIoU(const cv::Rect2f &rect1, const cv::Rect2f &rect2) {
    cv::Rect2f intersect = rect1 & rect2;
    float intersectArea = intersect.area();
    float unionArea = rect1.area() + rect2.area() - intersectArea;
    float iou = intersectArea / unionArea;
    return std::isnan(iou) ? 0.f : iou;
}

inline float calculateIoU(const cv::RotatedRect &rect1, const cv::RotatedRect &rect2) {
    std::vector<cv::Point2f> intersection_contour;
    cv::rotatedRectangleIntersection(rect1, rect2, intersection_contour);
    if (intersection_contour.empty())
        return 0;
    float intersectArea = float(cv::contourArea(intersection_contour));
    float unionArea = rect1.size.area() + rect2.size.area() - intersectArea;
    float iou = intersectArea / unionArea;
    return std::isnan(iou) ? 0.f : iou;
}


inline cv::Point3f cvMat2Point3f(const cv::Mat &mat) {
    assert(mat.rows == 3 && mat.cols == 1);
    cv::Mat float_mat;
    mat.convertTo(float_mat, CV_32F);
    return {float_mat.at<float>(0), float_mat.at<float>(1), float_mat.at<float>(2)};
}

inline Eigen::Vector3f cvPtToEigenVec3f(const cv::Point3f &pt) {
    return {pt.x, pt.y, pt.z};
}

inline cv::Point3f eigenVecToCvPt3f(const Eigen::Vector3f &pt) {
    return {pt.x(), pt.y(), pt.z()};
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

/**
 * Convert REP 103 to OpenCV coord represent.
 * @param pt REP 103
 * @return OpenCV point
 */
inline cv::Point3f repToOpencv(const cv::Point3f &pt) {
    return {-pt.y, -pt.z, pt.x};
}

#endif //CYGNOIDES_DECISION_CV_UTILS_H
