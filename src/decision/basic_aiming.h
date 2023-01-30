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
#include "utils/config.h"

class BasicAiming {
private:

    CameraCalib camera_calib;
    BallisticCompensator compensator{0.1};

    float ballet_init_speed = 15.0 * 1e3;
    float compensate_time = 0.0;

    int last_aiming_id_ = -1;
    EulerAngles last_aiming_angle_{};

    int chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, Time &predTime);

    EulerAngles predictFromTrack(ArmorTrack &track, Time predTime);

public:

    // Should be private. Set to public for debug use.
    Tracker tracker;
    Transformer transformer;

    /**
     * Initialize BasicAiming decision-maker.
     * @param camera_coeffs Path to camera calibration coeffs file.
     */
    explicit BasicAiming(ConfigLoader &config_loader, const std::string &camera_coeffs)
            : camera_calib(camera_coeffs), transformer(camera_calib), tracker(config_loader) {}

    /**
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     */
    EulerAngles update(DetectArmorsFrame &detection);

};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_H
