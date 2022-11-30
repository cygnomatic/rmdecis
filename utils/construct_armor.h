//
// Created by catslashbin on 22-11-23.
//

#ifndef CYGNOIDES_DECISION_CONSTRUCT_ARMOR_H
#define CYGNOIDES_DECISION_CONSTRUCT_ARMOR_H

#include "../typing/general.h"


#define SMALL_ARMOR_WIDTH 135.0f
#define SMALL_ARMOR_HEIGHT 125.0f
#define BIG_ARMOR_WIDTH 230.0f
#define BIG_ARMOR_HEIGHT 127.0f

enum ARMOR_TYPE
{
    SMALL_ARMOR = 0,
    BIG_ARMOR = 1
};

ArmorCorners3d get_armor_self_coord(const cv::Size_<float> &armor_size)
{
    return ArmorCorners3d(
            {{armor_size.width / 2,  armor_size.height / 2,  0},
             {-armor_size.width / 2, armor_size.height / 2,  0},
             {-armor_size.width / 2, -armor_size.height / 2, 0},
             {armor_size.width / 2,  -armor_size.height / 2, 0}}
    );
}

ArmorCorners3d get_armor_self_coord(ARMOR_TYPE armorType)
{
    switch (armorType)
    {
        case SMALL_ARMOR:
            return get_armor_self_coord({SMALL_ARMOR_WIDTH, SMALL_ARMOR_HEIGHT});
        case BIG_ARMOR:
            return get_armor_self_coord({BIG_ARMOR_WIDTH, BIG_ARMOR_HEIGHT});
    }
}

#endif //CYGNOIDES_DECISION_CONSTRUCT_ARMOR_H
