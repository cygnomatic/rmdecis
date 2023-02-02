//
// Created by catslashbin on 23-1-24.
//

#ifndef CYGNOIDES_DECISION_BASIC_AIMING_H
#define CYGNOIDES_DECISION_BASIC_AIMING_H

#include <map>

#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "../../include/typing.h"
#include "utils/cv_utils.h"
#include "compensator/ballistic_compensator.h"
#include "utils/config.h"

class BasicAiming {
private:

    CameraCalib camera_calib;
    BallisticCompensator compensator;

    float ballet_init_speed = 15.0 * 1e3;
    float compensate_time = 0.0;

    int last_aiming_id_ = -1;
    EulerAngles last_aiming_angle_{};

    int chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, Time &predTime);

    EulerAngles predictFromTrack(ArmorTrack &track, Time predTime);

public:

    // Should be private. Set to public for debug use.
    Tracker tracker;
    Reconstructor transformer;

    /**
     * Initialize BasicAiming decision-maker.
     * @param config_loader Config loader.
     */
    explicit BasicAiming(ConfigLoader &config_loader)
            : camera_calib(config_loader), transformer(camera_calib), tracker(config_loader),
              compensator(config_loader.load("aiming").get<float>("airResistanceConst", 0.1)) {

        Config cfg = config_loader.load("aiming");

        compensate_time = cfg.get<float>("basic.compensateTime", 0.0);
    }

    /**
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     */
    EulerAngles update(DetectArmorsFrame &detection);

};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_H
