//
// Created by catslashbin on 23-2-18.
//

#ifndef CYGNOIDES_DECISION_LARGE_RUNE_H
#define CYGNOIDES_DECISION_LARGE_RUNE_H

#include "typing.h"
#include "config.h"

class LargeRuneAiming {
public:
    /**
     * Initialize BasicAiming decision-maker.
     * @param cfg Config loader.
     */
    explicit LargeRuneAiming(Config &cfg);

    ~LargeRuneAiming();

    /**
     * Update BasicAiming decision-maker with frame and get where to aim.
     * @param detection Result from the detection part.
     * @param debug_frame Draw the debug info on this image. Pass in nullptr to disable debugging.
     * @return Euler angles, representing the aiming target.
     * @note The DetectArmorsFrame object is passed in by `move`. Do not reuse the detection.
     */
    EulerAngles update(RuneFrameInput detection, cv::Mat *debug_frame);

private:
    class LargeRuneAimingImpl;
    std::unique_ptr<LargeRuneAimingImpl> impl;
};

#endif //CYGNOIDES_DECISION_LARGE_RUNE_H
