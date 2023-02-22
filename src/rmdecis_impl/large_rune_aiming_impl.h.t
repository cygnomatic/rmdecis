//
// Created by catslashbin on 23-1-24.
//

#ifndef CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H
#define CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H

#include <map>
#include <Eigen/Dense>

#include "rmdecis/core.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "compensator/ballistic_compensator.h"
#include "utils/cv_utils.h"
#include "config.h"

#include "large_rune_aiming.h.t"

class LargeRuneAiming::LargeRuneAimingImpl {
private:

    BallisticCompensator compensator;

    float ballet_init_speed = 15.0 * 1e3;
    float compensate_time = 0.0;

    int last_aiming_id_ = -1;
    int n_updates = 0;
    EulerAngles last_aiming_angle_{};

    Eigen::Vector3f normal{0, 0, 0};
    Eigen::Vector3f rune_center_world;

    int chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, Time &predTime);

    EulerAngles predictFromTrack(ArmorTrack &track, Time predTime);

    float getAngleFromDetect(ArmorTrack &track);


public:

    // Should be private. Set to public for debug use.
    Tracker tracker;
    Transformer transformer;
    CameraCalib camera_calib;

    /**
     * Initialize BasicAimingImpl decision-maker.
     * @param cfg Config.
     */
    explicit LargeRuneAimingImpl(Config &cfg);

    /**
     * Update BasicAimingImpl decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     */
    EulerAngles update(FrameInput detection);

};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H
