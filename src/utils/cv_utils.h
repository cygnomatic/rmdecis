//
// Created by catslashbin on 23-1-3.
//

#ifndef CYGNOIDES_DECISION_CV_UTILS_H
#define CYGNOIDES_DECISION_CV_UTILS_H

#include "../typing/general.h"

float calculateIoU(const Rect2f &rect1, const Rect2f &rect2);

void drawArmorCorners(cv::Mat &image, ArmorCorners2d &corners, const cv::Scalar &color = Scalar(255, 0, 0),
                      int thickness = 2);

#endif //CYGNOIDES_DECISION_CV_UTILS_H
