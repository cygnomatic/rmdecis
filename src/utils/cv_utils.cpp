//
// Created by catslashbin on 23-1-6.
//

#include "cv_utils.h"

using namespace cv;

void drawArmorCorners(cv::Mat &image, ArmorCorners2d &corners, const cv::Scalar &color, int thickness) {
    cv::line(image, corners[0], corners[1], color, thickness);
    cv::line(image, corners[1], corners[2], color, thickness);
    cv::line(image, corners[2], corners[3], color, thickness);
    cv::line(image, corners[3], corners[0], color, thickness);
}

float calculateIoU(const Rect2f &rect1, const Rect2f &rect2) {
    Rect2f intersect = rect1 & rect2;
    float intersectArea = intersect.area();
    float unionArea = rect1.area() + rect2.area() - intersectArea;
    return intersectArea / unionArea;
}
