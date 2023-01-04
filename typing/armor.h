//
// Created by catslashbin on 22-11-28.
//

#ifndef CYGNOIDES_DECISION_ARMOR_H
#define CYGNOIDES_DECISION_ARMOR_H

#include "general.h"


#define SMALL_ARMOR_WIDTH 135.0f
#define SMALL_ARMOR_HEIGHT 125.0f
#define BIG_ARMOR_WIDTH 230.0f
#define BIG_ARMOR_HEIGHT 127.0f


struct ArmorInfo
{
    ArmorID armor_id;
    ArmorCorners3d corners_cam;
    ArmorCorners2d corners_img;
};

enum ARMOR_TYPE
{
    SMALL_ARMOR = 0,
    BIG_ARMOR = 1
};

ArmorCorners3d getArmorModelCoord(const cv::Size_<float> &armor_size)
{
    return ArmorCorners3d(
            {{armor_size.width / 2,  armor_size.height / 2,  0},
             {-armor_size.width / 2, armor_size.height / 2,  0},
             {-armor_size.width / 2, -armor_size.height / 2, 0},
             {armor_size.width / 2,  -armor_size.height / 2, 0}}
    );
}

ArmorCorners3d getArmorModelCoord(ARMOR_TYPE armorType)
{
    switch (armorType)
    {
        case SMALL_ARMOR:
            return getArmorModelCoord({SMALL_ARMOR_WIDTH, SMALL_ARMOR_HEIGHT});
        case BIG_ARMOR:
            return getArmorModelCoord({BIG_ARMOR_WIDTH, BIG_ARMOR_HEIGHT});
    }
}

#endif //CYGNOIDES_DECISION_ARMOR_H
