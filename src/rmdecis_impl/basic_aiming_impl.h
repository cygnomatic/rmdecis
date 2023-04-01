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
    float curr_pitch_ = 0.f;
    int compensate_frame = 0;

    int target_id_ = -1;
    int k_frame_width_, k_frame_height_;
    float k_confidence_threshold_;

    bool enable_show_vision_input, enable_show_tracker;

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
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @param debug_img Draw the debug info on this image. Pass in nullptr to disable debugging.
     * @return Euler angles, representing the aiming target.
     * @note The DetectArmorsFrame object is passed in by `move`. Do not reuse the detection.
     */
    EulerAngles update(ArmorFrameInput detection, cv::Mat *debug_img = nullptr);

};


#endif //CYGNOIDES_DECISION_BASIC_AIMING_IMPL_H
