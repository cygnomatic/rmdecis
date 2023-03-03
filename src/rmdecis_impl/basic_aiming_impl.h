//
// Created by catslashbin on 23-1-24.
//

#ifndef CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H
#define CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H

#include <map>

#include "rmdecis/core.h"
#include "tracker/tracker.h"
#include "reconstructor/reconstructor.h"
#include "compensator/ballistic_compensator.h"
#include "utils/cv_utils.h"
#include "config_impl.h"

#include "basic_aiming.h"

class BasicAiming::BasicAimingImpl {
private:

    BallisticCompensator compensator;

    float ballet_init_speed = 15.0 * 1e3;
    int compensate_frame = 0;

    int last_aiming_id_ = -1;
    int frame_width_, frame_height_;
    EulerAngles last_aiming_angle_{};

    int chooseNextTarget(std::map<int, ArmorTrack> &tracks_map, int frame_seq);

    EulerAngles predictFromTrack(ArmorTrack &track, int frame_seq);

public:

    // Should be private. Set to public for debug use.
    Tracker tracker;
    Reconstructor reconstructor;

    /**
     * Initialize BasicAimingImpl decision-maker.
     * @param cfg Config.
     */
    explicit BasicAimingImpl(Config &cfg);

    /**
     * Update BasicAimingImpl decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @return Euler angles, representing the aiming target.
     */
    EulerAngles update(ArmorFrameInput detection);

};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H
