//
// Created by catslashbin on 22-11-21.
//

#ifndef CYGNOIDES_DECISION_ARMOR_TRACKER_H
#define CYGNOIDES_DECISION_ARMOR_TRACKER_H

#include "../general/typing.h"

class ArmorTracker
{
public:
    cv::Point3f center;

    ArmorTracker();

    void updatePredPos(cv::Point3f center);
};


#endif //CYGNOIDES_DECISION_ARMOR_TRACKER_H
