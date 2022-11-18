//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_ARMOR_H
#define CYGNOIDES_DECISION_ARMOR_H

#include <opencv2/core.hpp>

class Armor
{
public:
    float width, height;
    std::vector<cv::Point2f> corners_img_coord; // from right up corner, counter-clockwise
    std::vector<cv::Point3f> corners_self_coord;
    std::vector<cv::Point3f> corners_cam_coord;

    Armor(float width, float height)
    {
        this->width = width; // In mm
        this->height = height;

        corners_self_coord.emplace_back(width / 2, height / 2, 0);
        corners_self_coord.emplace_back(-width / 2, height / 2, 0);
        corners_self_coord.emplace_back(-width / 2, -height / 2, 0);
        corners_self_coord.emplace_back(width / 2, -height / 2, 0);
    }

    cv::Point3f get_center_cam_coord()
    {
        cv::Point3f s;
        for (auto const &p: corners_cam_coord)
            s += p;
        return s / 4;
    }
};

class BigArmor : public Armor
{
public:
    BigArmor() : Armor(200.0, 100.0)
    {};
};

#endif //CYGNOIDES_DECISION_ARMOR_H
