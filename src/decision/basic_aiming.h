//
// Created by catslashbin on 23-1-24.
//

#ifndef CYGNOIDES_DECISION_BASIC_AIMING_H
#define CYGNOIDES_DECISION_BASIC_AIMING_H

#include <map>

#include "tracker/tracker.h"
#include "reconstructor/transformer.h"
#include "typing/core.h"
#include "utils/cv_utils.h"
#include "compensator/ballistic_compensator.h"

class BasicAiming {
// private:
public:

    Tracker tracker{0.75, 4, 3};
    CameraCalib camera_calib;
    Transformer transformer;
    BallisticCompensator compensator{0.1};

    float ballet_init_speed = 15.0 * 1e3;
    float compensate_time = 2.0;

    int last_aiming_id_ = -1;
    EulerAngles last_aiming_angle_{};

    int chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, Time &predTime);

// public:

    explicit BasicAiming(const std::string &camera_coeffs) : camera_calib(camera_coeffs), transformer(camera_calib) {}

    EulerAngles update(DetectArmorsFrame &detection);

    EulerAngles predictFromTrack(ArmorTrack &track, Time predTime);
};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_H
