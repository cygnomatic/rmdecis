//
// Created by catslashbin on 23-2-25.
//

#ifndef CYGNOIDES_DECISION_RUNE_RECONSTRUCTOR_H
#define CYGNOIDES_DECISION_RUNE_RECONSTRUCTOR_H

#include "rmdecis/core.h"
#include "reconstructor/camera_calib.h"
#include "reconstructor/transformer.h"

class RuneReconstructor {
public:

    Transformer transformer;
    CameraCalib cam_calib;

    explicit RuneReconstructor(Config &cfg);

    void reconstructVanes(std::vector<VaneInfo> vanes, const RobotState &robot_state);
};


#endif //CYGNOIDES_DECISION_RUNE_RECONSTRUCTOR_H
