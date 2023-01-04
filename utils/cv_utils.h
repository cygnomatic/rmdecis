//
// Created by catslashbin on 23-1-3.
//

#ifndef CYGNOIDES_DECISION_CV_UTILS_H
#define CYGNOIDES_DECISION_CV_UTILS_H

#include "../typing/general.h"

float calculateIoU(const Rect2f& rect1, const Rect2f& rect2)
{
    Rect2f intersect = rect1 & rect2;
    float intersectArea = intersect.area();
    float unionArea = rect1.area() + rect2.area() - intersectArea;
    return intersectArea / unionArea;
}

void drawArmorCorners(cv::Mat& image, ArmorCorners2d& corners, const cv::Scalar& color = Scalar(255, 0, 0))
{
    cv::line(image, corners[0], corners[1], color, 2);
    cv::line(image, corners[1], corners[2], color, 2);
    cv::line(image, corners[2], corners[3], color, 2);
    cv::line(image, corners[3], corners[0], color, 2);
}

#endif //CYGNOIDES_DECISION_CV_UTILS_H
