//
// Created by catslashbin on 22-11-18.
//

#ifndef CYGNOIDES_DECISION_ARMOR_H
#define CYGNOIDES_DECISION_ARMOR_H

#include <opencv2/core.hpp>
#include "../../general/typing.h"


class Armor
{
public:
    float width, height;

    ArmorCorners2d corners_img_coord;
    ArmorCorners3d corners_self_coord;
    ArmorCorners3d corners_cam_coord;

    Armor(float width, float height)
    {
        this->width = width; // In millimeter
        this->height = height;

        corners_self_coord.tr = {width / 2, height / 2, 0};
        corners_self_coord.tl = {-width / 2, height / 2, 0};
        corners_self_coord.dl = {-width / 2, -height / 2, 0};
        corners_self_coord.dr = {width / 2, -height / 2, 0};
    }
};

class BigArmor : public Armor
{
public:
    BigArmor() : Armor(200.0, 100.0)
    {};
};

#endif //CYGNOIDES_DECISION_ARMOR_H
