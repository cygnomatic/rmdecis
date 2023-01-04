//
// Created by catslashbin on 23-1-3.
//

#ifndef CYGNOIDES_DECISION_CV_UTILS_H
#define CYGNOIDES_DECISION_CV_UTILS_H

#include <opencv2/core/types.hpp>

float calculateIoU(const cv::Rect2f& rect1, const cv::Rect2f& rect2)
{
    cv::Rect2f intersect = rect1 & rect2;
    float intersectArea = intersect.area();
    float unionArea = rect1.area() + rect2.area() - intersectArea;
    return intersectArea / unionArea;
}


#endif //CYGNOIDES_DECISION_CV_UTILS_H
