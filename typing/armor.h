//
// Created by catslashbin on 22-11-28.
//

#ifndef CYGNOIDES_DECISION_ARMOR_H
#define CYGNOIDES_DECISION_ARMOR_H

#include "general.h"


struct ArmorInfo
{
    ArmorID armor_id;
    ArmorCorners3d corners_cam;
    ArmorCorners2d corners_img;

    // Point3f center_cam;
    // Point2f center_img;
};

#endif //CYGNOIDES_DECISION_ARMOR_H
